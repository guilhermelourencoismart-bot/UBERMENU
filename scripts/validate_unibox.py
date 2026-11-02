"""Static project/package preflight; does not compile or execute Unibox."""
import argparse
from pathlib import Path
import xml.etree.ElementTree as ET


def validate(root, require_packages=False):
    errors = []
    project = root / 'Unibox/Unibox.vcxproj'
    optional = {
        'src/Features/Misc/ProfileStalker/ProfileStalker.cpp',
        'src/Features/Misc/ProfileStalker/ProfileStalker.h',
    }
    present = {name for name in optional if (project.parent / name).is_file()}
    if present and present != optional:
        errors.append('ProfileStalker is incomplete: restore the original .cpp/.h pair.')
    elif not present:
        print('WARNING: ProfileStalker sources are not distributed; original optional integration retained.')

    count = 0
    for filename in (project, project.with_suffix('.vcxproj.filters')):
        seen = set()
        for item in ET.parse(filename).getroot().iter():
            kind = item.tag.rsplit('}', 1)[-1]
            name = item.get('Include', '').replace('\\', '/')
            if kind not in {'ClCompile', 'ClInclude', 'None', 'ResourceCompile', 'Image'} or not name:
                continue
            count += 1
            key = (kind, name.casefold())
            if key in seen:
                errors.append(f'{filename.name}: duplicate {kind} reference {name}')
            seen.add(key)
            if '$(' in name or '*' in name:
                errors.append(f'{filename.name}: cannot statically resolve {name}')
            elif not (filename.parent / name).is_file():
                if name not in optional or not item.get('Condition'):
                    errors.append(f'{filename.name}: missing file {name}')

    ns = {'m': 'http://schemas.microsoft.com/developer/msbuild/2003'}
    tree = ET.parse(project)
    # CL defaults to $(IntDir)%(Filename).obj: distinct source directories can
    # silently overwrite each other's objects before the linker sees them.
    objects = {}
    for source in tree.findall('.//m:ClCompile[@Include]', ns):
        name = source.get('Include').replace('\\', '/')
        target = source.findtext('m:ObjectFileName', namespaces=ns)
        target = target or ('$(IntDir)' + Path(name).stem + '.obj')
        target = target.replace('$(IntDir)', '').replace('\\', '/').casefold()
        if target in objects:
            errors.append(f'Object output collision: {objects[target]} and {name} -> {target}')
        objects[target] = name
    group = tree.find("m:PropertyGroup[@Label='Configuration'][@Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\"]", ns)
    if group is None or group.findtext('m:PlatformToolset', namespaces=ns) != 'v143':
        errors.append('Release|x64 must use MSVC v143.')
    solution = (root / 'Unibox.sln').read_text(encoding='utf-8-sig')
    if '.Release|x64.Build.0 = Release|x64' not in solution:
        errors.append('Solution does not enable Release|x64 build.')

    config = ET.parse(root / 'NuGet.config')
    if config.find('./config/add[@key="repositoryPath"]').get('value') != 'packages':
        errors.append('NuGet repositoryPath must match the project packages directory.')
    for package in ET.parse(project.parent / 'packages.config').getroot():
        name, version = package.get('id'), package.get('version')
        print(f'NuGet: {name} {version}')
        if require_packages:
            directory = root / 'packages' / f'{name}.{version}'
            if not directory.is_dir():
                errors.append(f'Package not restored: {directory}')
            if name == 'boost':
                for relative in ('build/boost.targets', 'lib/native/include/boost/version.hpp'):
                    if not (directory / relative).is_file():
                        errors.append(f'Missing Boost file: {directory / relative}')
    print(f'Checked {count} project/filter file references.')
    return errors


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--require-packages', action='store_true')
    args = parser.parse_args()
    failures = validate(Path(__file__).resolve().parents[1], args.require_packages)
    for failure in failures:
        print(f'ERROR: {failure}')
    raise SystemExit(bool(failures))
