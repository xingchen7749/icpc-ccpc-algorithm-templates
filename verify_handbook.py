"""Check generated navigation, method files, and reproducible generation."""
from pathlib import Path
from urllib.parse import unquote
import hashlib
import json
import re
import subprocess
import sys

from build_handbook import CODE_PATHS, GROUPS, NUMBERS, OUT, ROOT
from handbook_data import ENTRIES


def check():
    expected = {p.as_posix() for p in CODE_PATHS.values()}
    actual = {p.relative_to(OUT/'代码').as_posix() for p in (OUT/'代码').rglob('*.cpp')}
    assert actual == expected, ('unexpected or missing code', actual ^ expected)
    manifest = json.loads((OUT/'代码文件清单.json').read_text(encoding='utf-8'))
    assert len(manifest) == len(set(manifest)) and set(manifest) == expected
    documents = [OUT/'算法模板手册.md', *sorted((OUT/'章节').glob('*.md'))]
    all_keys = {e['key'] for e in ENTRIES}
    for path in documents:
        content = path.read_text(encoding='utf-8')
        # Only inspect rendered prose, not C++ literals and comments.
        prose = re.sub(r'```[^\n]*\n.*?```', '', content, flags=re.S)
        ids = re.findall(r'<a id="([^"]+)"></a>', prose)
        assert len(ids) == len(set(ids)), (path, 'duplicate anchor')
        links = re.findall(r'\[[^\]\n]+\]\(([^)\s]+)\)', prose)
        for link in links:
            if link.startswith('#'):
                assert link[1:] in ids, (path, link)
            elif '://' not in link:
                target = unquote(link.split('#', 1)[0])
                assert (path.parent/target).exists(), (path, link)
                if '#' in link:
                    anchor = unquote(link.split('#', 1)[1])
                    destination = (path.parent/target).read_text(encoding='utf-8')
                    assert f'<a id="{anchor}"></a>' in destination, (path, link)
        keys = {s[len('algorithm-'):] for s in ids if s.startswith('algorithm-')}
        if path == documents[0]:
            assert keys == all_keys
            assert {f'chapter-{i}' for i in range(1, len(GROUPS)+1)} <= set(ids)
        else:
            chapter = path.stem.split('_', 1)[1]
            assert keys == {e['key'] for e in ENTRIES if e['chapter'] == chapter}
        for key in keys:
            toc_links = re.findall(r'^\s*- \[[^\]\n]+\]\(#(algorithm-[^)]+)\)', prose, re.M)
            assert toc_links.count('algorithm-' + key) == 1, (path, key, 'missing or duplicate TOC link')
            e = next(e for e in ENTRIES if e['key'] == key)
            title = f'{NUMBERS[key]} {e["title"]}'
            assert re.search(r'^#{2,3} ' + re.escape(title) + r'$', prose, re.M)
        assert not re.search(r'\{\{(?:code|code-link|section|dependency-link):', content)
    for key, relative in CODE_PATHS.items():
        source = (OUT/'代码'/relative).read_text(encoding='utf-8')
        assert source.startswith('#include <bits/stdc++.h>'), key
        assert source.count('void solve()') == 1, key
        assert 'while (T--) solve();' in source, key
        assert source.count('signed main()') == 1, key
    print(f'Navigation and filenames OK: {len(ENTRIES)} programs, {len(documents)} documents.')


def snapshot():
    return {str(p.relative_to(OUT)): hashlib.sha256(p.read_bytes()).hexdigest()
            for p in OUT.rglob('*') if p.is_file()}


if __name__ == '__main__':
    check()
    before = snapshot()
    subprocess.run([sys.executable, str(ROOT/'build_handbook.py')], check=True)
    assert snapshot() == before, 'regeneration changed file contents or file set'
    print('Regeneration is stable.')
