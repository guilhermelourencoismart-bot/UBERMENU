"""Regenerate the display-only C++ catalog from the reviewed PT-BR JSON."""
import argparse
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def generate(check=False):
    catalog = json.loads((ROOT / 'docs/PT-BR-LABELS.json').read_text(encoding='utf-8'))
    path = ROOT / 'Unibox/src/Features/ImGui/Menu/Localization.h'
    original = path.read_text(encoding='utf-8')
    begin = original.index(' catalog={') + len(' catalog={')
    end = original.index('\n};', begin)
    entries = '\n' + ''.join(' {' + json.dumps(key, ensure_ascii=False) + ',' + json.dumps(value, ensure_ascii=False) + '},\n' for key, value in catalog.items())
    generated = original[:begin] + entries + original[end:]
    if check and generated != original:
        raise ValueError('C++ captions differ from PT-BR-LABELS.json; regenerate the catalog')
    if not check:
        path.write_text(generated, encoding='utf-8')
    print(f'{len(catalog)} display captions: ' + ('catalog synchronized' if check else 'catalog generated'))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    generate(args.check)
