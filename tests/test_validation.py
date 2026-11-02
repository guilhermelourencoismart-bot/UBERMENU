import contextlib
import importlib.util
import io
from pathlib import Path
import shutil
import struct
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


def module(name):
    spec = importlib.util.spec_from_file_location(name, ROOT / 'scripts' / f'{name}.py')
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


preflight = module('validate_unibox')
pe = module('verify_pe')


class PreflightTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name) / 'source'
        shutil.copytree(ROOT, self.root, ignore=shutil.ignore_patterns('.git', '__pycache__'))

    def check(self, packages=False):
        with contextlib.redirect_stdout(io.StringIO()):
            return preflight.validate(self.root, packages)

    def test_actual_project(self):
        self.assertEqual(self.check(), [])

    def test_missing_source(self):
        (self.root / 'Unibox/src/Features/Visuals/Radar/Radar.cpp').unlink()
        self.assertTrue(any('Radar.cpp' in error for error in self.check()))

    def test_partial_optional_module(self):
        folder = self.root / 'Unibox/src/Features/Misc/ProfileStalker'
        folder.mkdir(parents=True, exist_ok=True)
        (folder / 'ProfileStalker.h').write_text('// temporary test fixture')
        self.assertTrue(any('incomplete' in error for error in self.check()))

    def test_duplicate_source(self):
        path = self.root / 'Unibox/Unibox.vcxproj'
        text = path.read_text(encoding='utf-8-sig').replace('</Project>', '<ItemGroup><ClCompile Include="src\\Features\\Visuals\\Radar\\Radar.cpp" /></ItemGroup></Project>')
        path.write_text(text, encoding='utf-8')
        self.assertTrue(any('duplicate' in error for error in self.check()))

    def test_object_basename_collision(self):
        source = self.root / 'Unibox/src/Features/Misc/Radar.cpp'
        source.write_text('// deliberately conflicting source fixture')
        path = self.root / 'Unibox/Unibox.vcxproj'
        text = path.read_text(encoding='utf-8-sig').replace('</Project>', '<ItemGroup><ClCompile Include="src/Features/Misc/Radar.cpp" /></ItemGroup></Project>')
        path.write_text(text, encoding='utf-8')
        self.assertTrue(any('Object output collision' in error for error in self.check()))

    def test_restore_required(self):
        shutil.rmtree(self.root / 'packages', ignore_errors=True)
        self.assertTrue(any('Package not restored' in error for error in self.check(True)))


class PETests(unittest.TestCase):
    def image(self, machine=0x8664, flags=0x2000, magic=0x20B):
        data = bytearray(512)
        data[:2] = b'MZ'
        struct.pack_into('<I', data, 60, 128)
        data[128:132] = b'PE\0\0'
        struct.pack_into('<HH', data, 132, machine, 1)
        struct.pack_into('<HH', data, 148, 240, flags)
        struct.pack_into('<H', data, 152, magic)
        return data

    def test_amd64_dll_header(self):
        pe.verify(self.image())

    def test_wrong_architecture(self):
        with self.assertRaisesRegex(ValueError, 'AMD64'): pe.verify(self.image(machine=0x14C))

    def test_executable_not_dll(self):
        with self.assertRaisesRegex(ValueError, 'DLL'): pe.verify(self.image(flags=2))

    def test_truncated(self):
        with self.assertRaises(ValueError): pe.verify(self.image()[:160])

    def test_wrong_magic(self):
        with self.assertRaisesRegex(ValueError, 'PE32'): pe.verify(self.image(magic=0x10B))


if __name__ == '__main__':
    unittest.main()
