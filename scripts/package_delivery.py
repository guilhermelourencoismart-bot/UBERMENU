"""Create reproducible source or verified Windows binary delivery ZIPs."""
import argparse
import hashlib
import io
import json
import os
from pathlib import Path
import subprocess
import tempfile
import zipfile
from validate_unibox import validate
from verify_pe import verify

ROOT = Path(__file__).resolve().parents[1]
REQUIRED_DOCS = ('BUILD.md', 'FEATURES.md', 'INTEGRATION-STATUS.md', 'TEST-RESULTS.md',
                 'KNOWN-ISSUES.md', 'THIRD-PARTY-NOTICES.md', 'LOCAL-TESTING.md', 'KNOWN-RUNTIME-RISKS.md')


def source_package(output, revision):
    archive = subprocess.check_output(['git', 'archive', '--format=zip', revision], cwd=ROOT)
    # Validate the archived snapshot, independently of untracked files in this checkout.
    with tempfile.TemporaryDirectory() as directory:
        with zipfile.ZipFile(io.BytesIO(archive)) as zipped:
            zipped.extractall(directory)
        failures = validate(Path(directory))
        for name in REQUIRED_DOCS:
            if not (Path(directory) / 'docs' / name).is_file():
                failures.append(f'Missing delivery document: {name}')
        for name in ('LICENSE.md', 'docs/licenses/Amalgam-Comp-LICENSE.md', 'docs/licenses/Amalgam-v2-LICENSE.md'):
            if not (Path(directory) / name).is_file():
                failures.append(f'Missing license: {name}')
        if failures:
            raise ValueError('\n'.join(failures))
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_bytes(archive)


def binary_package(output, dll):
    verify(dll.read_bytes())
    commit = subprocess.check_output(['git', 'rev-parse', 'HEAD'], cwd=ROOT, text=True).strip()
    files = [dll]
    pdb = dll.with_suffix('.pdb')
    if pdb.is_file():
        files.append(pdb)
    files += [ROOT / 'docs' / name for name in REQUIRED_DOCS]
    files += [ROOT / 'LICENSE.md', *sorted((ROOT / 'docs' / 'licenses').glob('*'))]
    output.parent.mkdir(parents=True, exist_ok=True)
    with zipfile.ZipFile(output, 'w', compression=zipfile.ZIP_DEFLATED) as zipped:
        for path in files:
            zipped.write(path, path.name if path in (dll, pdb) else str(path.relative_to(ROOT)))
        manifest = {'commit': commit, 'configuration': dll.parent.name, 'dll': dll.name,
                    'dll_sha256': hashlib.sha256(dll.read_bytes()).hexdigest(),
                    'pe': 'PE32+/AMD64/DLL', 'runtime_tested_in_TF2': False}
        if os.environ.get('GITHUB_RUN_ID'):
            manifest['workflow'] = f"{os.environ['GITHUB_SERVER_URL']}/{os.environ['GITHUB_REPOSITORY']}/actions/runs/{os.environ['GITHUB_RUN_ID']}"
        zipped.writestr('BUILD-MANIFEST.json', json.dumps(manifest, indent=2) + '\n')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    mode = parser.add_mutually_exclusive_group(required=True)
    mode.add_argument('--source', metavar='REVISION')
    mode.add_argument('--dll', type=Path)
    args = parser.parse_args()
    if args.source:
        source_package(args.output, args.source)
    else:
        binary_package(args.output, args.dll)
    print(f'{args.output}: {args.output.stat().st_size} bytes; SHA256 {hashlib.sha256(args.output.read_bytes()).hexdigest()}')
