"""Generate Markdown and self-contained C++ contest programs. No PDF is produced."""
from pathlib import Path
from collections import defaultdict
from urllib.parse import quote
from handbook_data import ENTRIES

ROOT = Path(__file__).resolve().parent
OUT = ROOT / 'ICPC_CCPC算法模板'
STARTER = next(e['code'] for e in ENTRIES if e['key'] == 'starter')
PRELUDE, ENTRYPOINT = STARTER.split('void solve()', 1)
BY_KEY = {e['key']: e for e in ENTRIES}
GROUPS = defaultdict(list)
for item in ENTRIES:
    GROUPS[item['chapter']].append(item)

def filename_title(title):
    """Keep handbook titles readable while making them legal Windows filenames."""
    return title.translate(str.maketrans('<>:"/\\|?*', '＜＞：＂／＼｜？＊')).rstrip(' .')

CODE_PATHS = {
    item['key']: Path(f'{i:02d}_{filename_title(chapter)}') / f'{i}.{j} {filename_title(item["title"])}.cpp'
    for i, (chapter, items) in enumerate(GROUPS.items(), 1)
    for j, item in enumerate(items, 1)
}
SEGMENTTREE_COMBINED_PATH = CODE_PATHS['lazyseg']
CODE_PATHS['lazyseg'] = SEGMENTTREE_COMBINED_PATH.with_name('2.5.1 带懒标记线段树（segmenttree）.cpp')
CODE_PATHS['segtree'] = SEGMENTTREE_COMBINED_PATH.with_name('2.5.2 无懒标记线段树（SegTree）.cpp')
INVERSE_COMBINED_PATH = CODE_PATHS['modular']
CODE_PATHS['fermat'] = INVERSE_COMBINED_PATH.with_name('5.2.1 费马小定理求逆元.cpp')
CODE_PATHS['modular'] = INVERSE_COMBINED_PATH.with_name('5.2.2 扩展欧几里得求逆元.cpp')
BY_KEY['fermat'] = dict(code=BY_KEY['modular']['code'])
if len({path.as_posix().casefold() for path in CODE_PATHS.values()}) != len(CODE_PATHS):
    raise ValueError('Duplicate algorithm output paths')

def render_code_paths(text):
    for key, path in CODE_PATHS.items():
        text = text.replace(f'{{{{code:{key}}}}}', '代码/' + path.as_posix())
    return text

def render_code_links(text, prefix=''):
    for key, path in CODE_PATHS.items():
        target = quote(prefix + '代码/' + path.as_posix(), safe='/')
        text = text.replace(f'{{{{code-link:{key}}}}}', f'[完整代码：{path.name}]({target})')
    return text

def implementation_sections(chapter_number, item_number, item):
    """Share implementation titles and stable anchors between contents and body."""
    prefix = f'{chapter_number}.{item_number}'
    sections = []
    title = item.get('subsection_title') or item.get('code_title')
    if title:
        if item.get('numbered_subsections'):
            title = f'{prefix}.1 {title}'
        sections.append((title, f'algorithm-{item["key"]}-1'))
    for index, variant in enumerate(item.get('variants', []), 2):
        title = variant['title']
        if item.get('numbered_subsections'):
            title = f'{prefix}.{index} {title}'
        sections.append((title, f'algorithm-{item["key"]}-{index}'))
    return sections

def algorithm_contents(chapter_number, items):
    """List every entry and implementation without chapter-only navigation."""
    lines = []
    for j, item in enumerate(items, 1):
        lines.append(f'- [{chapter_number}.{j} {item["title"]}](#algorithm-{item["key"]})')
        for title, anchor in implementation_sections(chapter_number, j, item):
            lines.append(f'  - [{title}](#{anchor})')
    return '\n'.join(lines) + '\n'

DEPS = {
    'kruskal': ('dsu', ['init', 'find', 'merge']),
    'fermat': ('qpow', ['qpow']),
    'comb': ('qpow', ['qpow']),
    'ntt': ('qpow', ['qpow']),
    'twosat': ('scc', ['SCC']),
    'virtual': ('hld', ['HLD']),
    'crt': ('modular', ['exgcd']),
    'polygon': ('intgeo', ['Point', 'cross', 'on_segment']),
    'halfplane': ('floatgeo', ['P', 'ld', 'cross', 'dot', 'EPS']),
}

def algorithm_body(key):
    item = BY_KEY[key]
    if key == 'modular':
        return render_code_paths(item['variants'][0]['code'])
    return render_code_paths('\n\n'.join([item['code']] + [v['code'] for v in item.get('variants', [])]))

def algorithm_blocks(key, emitted=None, active=None):
    """Expand dependencies once when generating a standalone contest file."""
    if emitted is None:
        emitted = set()
    if active is None:
        active = set()
    if key in active:
        raise ValueError(f'Cyclic algorithm dependency: {key}')
    if key in emitted:
        return ''
    active.add(key)
    result = ''
    if key in DEPS:
        dep, _ = DEPS[key]
        result += algorithm_blocks(dep, emitted, active)
    result += algorithm_body(key) + '\n\n'
    active.remove(key)
    emitted.add(key)
    return result

def standalone_source(key):
    if 'solve' in BY_KEY.get(key, {}):
        main = 'signed main()' + ENTRYPOINT.split('signed main()', 1)[1]
        return PRELUDE.rstrip() + '\n\n' + algorithm_blocks(key) + BY_KEY[key]['solve'] + '\n\n' + main + '\n'
    if key in ('lazyseg', 'segtree'):
        item = BY_KEY['lazyseg']
        body = item['code'] if key == 'lazyseg' else item['variants'][0]['code']
        return PRELUDE.rstrip() + '\n\n' + body + '\n\nvoid solve()' + ENTRYPOINT + '\n'
    return PRELUDE.rstrip() + '\n\n' + algorithm_blocks(key) + 'void solve()' + ENTRYPOINT + '\n'

def verification_header():
    """Keep namespace isolation in test infrastructure, outside contest templates."""
    text = '// 自动生成，仅供测试；写题请使用代码目录中的独立程序。\n#pragma once\n' + PRELUDE
    emitted = set()
    def append(key):
        nonlocal text
        if key in emitted:
            return
        imports = ''
        if key in DEPS:
            dep, names = DEPS[key]
            append(dep)
            imports = ''.join(f'using cp_{dep}::{name};\n' for name in names)
        # Check that the exact body used in the adapter exists in the generated program.
        source = (OUT/'代码'/CODE_PATHS[key]).read_text(encoding='utf-8')
        body = algorithm_body(key)
        if key == 'lazyseg':
            plain_source = (OUT/'代码'/CODE_PATHS['segtree']).read_text(encoding='utf-8')
            if BY_KEY[key]['code'] not in source or BY_KEY[key]['variants'][0]['code'] not in plain_source:
                raise ValueError('Generated segment tree body mismatch')
        elif body not in source:
            raise ValueError(f'Generated algorithm body mismatch: {key}')
        if key != 'lazyseg':
            body = f'namespace cp_{key} {{\n' + imports + body + '\n}\n'
        text += '\n' + body + '\n'
        emitted.add(key)
    for key in BY_KEY:
        if key != 'starter':
            append(key)
    return text

def main():
    (OUT/'代码').mkdir(parents=True, exist_ok=True)
    (OUT/'章节').mkdir(exist_ok=True)
    (OUT/'测试').mkdir(exist_ok=True)
    groups=GROUPS
    full=['# ICPC / CCPC 算法模板手册\n',
          '> C++17 · 个人可编辑版 v1.3 · 2026-10-09\n',
          '## 阅读约定\n',
          '- 普通数组、图和树顶点使用 **1..n**；数组查询为闭区间 **[l,r]**。普通输入数组分配 n+1 个位置，忽略 a[0]。字符串仍从 0 开始。\n'
          '- 下标用 `int`；`i64`、`i128`、`u64` 分别为 `long long`、`__int128_t`、`unsigned long long` 的别名。`// #define int long long` 保持注释，默认不启用。\n'
          '- 所有代码片段依赖第一节公共头文件，使用整数别名时还需保留对应的 using 定义；少数模板还依赖其他条目，已明确标注。\n'
          '- 字符串、多项式次数、位掩码和 DP 容量等按数学含义保留 0；无占位列表和内部缓冲区的例外在各节注明。\n'
          '- “模板能编译”不代表可以无条件套用。使用前检查数值范围、前置条件和复杂度。\n'
          '- 本手册按竞赛使用场景重新组织、编写；参考 PDF 仅用于确认手册形式，未逐页复制。\n'
          '- `代码/` 按本手册章节分目录，文件名为“章节序号 算法标题.cpp”；Windows 不允许的文件名字符改用全角字符。每份算法均带完整起手式及展开的依赖，直接使用全局函数或类名，在 `solve()` 中写题并单独编译；不包含其他算法文件。手册代码片段省略起手式。\n',
          '## 目录\n']
    for i,(chapter,items) in enumerate(groups.items(),1):
        full.append(f'- [{i}. {chapter}](#chapter-{i})（{len(items)} 项）\n' +
                    '\n'.join('  ' + line for line in algorithm_contents(i, items).splitlines()) + '\n')
    headers=[]
    for i,(chapter,items) in enumerate(groups.items(),1):
        chunk=[f'<a id="chapter-{i}"></a>\n\n# {i}. {chapter}\n']
        for j,e in enumerate(items,1):
            key=e['key']
            sections = dict((anchor, title) for title, anchor in implementation_sections(i, j, e))
            chunk.append(f'\n<a id="algorithm-{key}"></a>\n\n## {i}.{j} {e["title"]}\n')
            if key not in ('lazyseg', 'modular'):
                chunk.append('{{code-link:' + key + '}}\n')
            if e.get('intro'):
                chunk.append(e['intro'])
            if e.get('numbered_subsections'):
                chunk.append(f'<a id="algorithm-{key}-1"></a>\n\n### {sections[f"algorithm-{key}-1"]}\n')
                if key == 'lazyseg':
                    chunk.append('{{code-link:lazyseg}}\n')
            if e.get('subsection_title'):
                chunk.append(f'<a id="algorithm-{key}-1"></a>\n\n### {sections[f"algorithm-{key}-1"]}\n')
                if key == 'modular':
                    chunk.append('{{code-link:fermat}}\n')
            chunk.extend([f'**用途与条件**：{e["use"]}\n',
                          f'**复杂度**：{e["cost"]}\n',
                          f'**易错点**：{e["pitfalls"]}\n'])
            if e.get('code_title') and not e.get('numbered_subsections'):
                chunk.append(f'<a id="algorithm-{key}-1"></a>\n\n### {sections[f"algorithm-{key}-1"]}\n')
            chunk.extend([
                          f'```cpp\n{e["code"]}\n```\n',
                          f'**调用与说明**：{e["example"]}\n'])
            if e.get('code_after'):
                chunk.append(e['code_after'])
            for variant_index, variant in enumerate(e.get('variants', []), 2):
                anchor = f'algorithm-{key}-{variant_index}'
                chunk.append(f'<a id="{anchor}"></a>\n\n### {sections[anchor]}\n')
                if key == 'lazyseg':
                    chunk.append('{{code-link:segtree}}\n')
                if key == 'modular':
                    chunk.append('{{code-link:modular}}\n')
                for field, label in [('use', '用途与条件'), ('cost', '复杂度'), ('pitfalls', '易错点')]:
                    if field in variant:
                        chunk.append(f'**{label}**：{variant[field]}\n')
                chunk.extend([f'```cpp\n{variant["code"]}\n```\n',
                              f'**调用与说明**：{variant["example"]}\n'])
                if variant.get('after'):
                    chunk.append(variant['after'])
            if e.get('after'):
                chunk.append(e['after'])
            destination = OUT / '代码' / CODE_PATHS[key]
            destination.parent.mkdir(parents=True, exist_ok=True)
            source = e['code']+'\n' if key == 'starter' else standalone_source(key)
            destination.write_text(source, encoding='utf-8')
            if key != 'starter':
                headers.append(CODE_PATHS[key].as_posix())
            if key == 'lazyseg':
                (OUT/'代码'/CODE_PATHS['segtree']).write_text(standalone_source('segtree'), encoding='utf-8')
                headers.append(CODE_PATHS['segtree'].as_posix())
            if key == 'modular':
                (OUT/'代码'/CODE_PATHS['fermat']).write_text(standalone_source('fermat'), encoding='utf-8')
                headers.append(CODE_PATHS['fermat'].as_posix())
        content=render_code_paths('\n'.join(chunk))
        chapter_content = render_code_paths('\n'.join([
            chunk[0], '## 本章算法目录\n', algorithm_contents(i, items), *chunk[1:]
        ]))
        (OUT/f'章节/{i:02d}_{chapter}.md').write_text(render_code_links(chapter_content, '../'),encoding='utf-8')
        full.append(content)
    appendix=(ROOT/'appendix.md').read_text(encoding='utf-8')
    full.append(appendix)
    (OUT/'算法模板手册.md').write_text(render_code_links(render_code_paths('\n'.join(full))),encoding='utf-8')
    (OUT/'测试/algorithm_adapters.hpp').write_text(verification_header(), encoding='utf-8')
    (OUT/'测试/segmenttree_adapter.hpp').write_text('// 自动生成，仅供线段树专项测试。\n#pragma once\n' + algorithm_body('lazyseg') + '\n', encoding='utf-8')
    combined_path = (OUT/'代码'/SEGMENTTREE_COMBINED_PATH).resolve()
    expected_parent = (OUT/'代码'/CODE_PATHS['lazyseg'].parent).resolve()
    if combined_path.parent != expected_parent or combined_path.name != '2.5 线段树.cpp':
        raise ValueError('Unexpected combined segment tree path')
    combined_path.unlink(missing_ok=True)
    inverse_combined = (OUT/'代码'/INVERSE_COMBINED_PATH).resolve()
    if inverse_combined.parent != (OUT/'代码'/CODE_PATHS['modular'].parent).resolve() or inverse_combined.name != '5.2 逆元.cpp':
        raise ValueError('Unexpected combined inverse path')
    inverse_combined.unlink(missing_ok=True)
    # Remove only known flat-layout files after their replacements have been written.
    code_dir = (OUT/'代码').resolve()
    legacy_names = ['contest.cpp', 'all.hpp', 'common.hpp'] + [f'{key}.{ext}' for key in BY_KEY if key != 'starter' for ext in ('cpp', 'hpp')]
    for filename in legacy_names:
        obsolete = (code_dir / filename).resolve()
        if obsolete.parent != code_dir:
            raise ValueError(f'Unexpected output path: {obsolete}')
        obsolete.unlink(missing_ok=True)
    print(f'Generated {len(ENTRIES)} entries, {len(headers)} algorithm files, {len(groups)} chapters in {OUT}')

if __name__=='__main__': main()
