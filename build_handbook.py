"""Generate the handbook and one self-contained C++ program per method."""
from pathlib import Path
from collections import OrderedDict
from urllib.parse import quote
import json
import re
from handbook_data import ENTRIES, HELPERS

ROOT = Path(__file__).resolve().parent
OUT = ROOT / 'ICPC_CCPC算法模板'
BY_KEY = {e['key']: e for e in ENTRIES}
STARTER = BY_KEY['starter']['code']
PRELUDE, ENTRYPOINT = STARTER.split('void solve()', 1)
GROUPS = OrderedDict()
for e in ENTRIES:
    sections = GROUPS.setdefault(e['chapter'], OrderedDict())
    sections.setdefault(e.get('section', e['title']), []).append(e)


def filename_title(title):
    return title.translate(str.maketrans('<>:"/\\|?*', '＜＞：＂／＼｜？＊')).rstrip(' .')


CODE_PATHS, NUMBERS = {}, {}
for i, (chapter, sections) in enumerate(GROUPS.items(), 1):
    for j, (title, methods) in enumerate(sections.items(), 1):
        for k, e in enumerate(methods, 1):
            number = f'{i}.{j}' + (f'.{k}' if 'section' in e else '')
            NUMBERS[e['key']] = number
            CODE_PATHS[e['key']] = Path(f'{i:02d}_{filename_title(chapter)}') / f'{number} {filename_title(e["title"])}.cpp'
if len({p.as_posix().casefold() for p in CODE_PATHS.values()}) != len(CODE_PATHS):
    raise ValueError('Duplicate output path')

# Dependencies are expanded in standalone programs and imported in test adapters.
DEPS = {
    'kruskal': [('dsu', ['init', 'find', 'merge'])],
    'fermat': [('qpow', ['qpow'])],
    'comb': [('qpow', ['qpow'])],
    'lucas': [('comb', ['Comb'])],
    'ntt': [('qpow', ['qpow'])],
    'twosat': [('scc', ['SCC'])],
    'virtual': [('hld', ['HLD'])],
    'tree_point_diff': [('hld', ['HLD'])],
    'tree_edge_diff': [('hld', ['HLD'])],
    'crt': [('modular', ['exgcd'])],
    'factor': [('primality', ['u128', 'mul', 'pw', 'is_prime'])],
    'bounded': [('knapsack', ['zero_one'])],
    'intgeo': [('geo_int', ['Point', 'cross', 'sign', 'on_segment'])],
    'hull': [('geo_int', ['Point', 'cross'])],
    'polygon': [('geo_int', ['Point'])],
    'point_location': [('geo_int', ['Point', 'cross', 'on_segment'])],
    'diameter': [('geo_int', ['Point', 'cross'])],
    'floatgeo': [('geo_float', ['P', 'ld', 'cross', 'dot', 'EPS'])],
    'circle': [('geo_float', ['P', 'ld', 'cross', 'dot', 'EPS'])],
    'halfplane': [('geo_float', ['P', 'ld', 'cross', 'dot', 'EPS'])],
}


def render_code_paths(text):
    for key, path in CODE_PATHS.items():
        text = text.replace('{{code:' + key + '}}', '代码/' + path.as_posix())
        text = text.replace('{{section:' + key + '}}', NUMBERS[key])
    return text


def render_code_links(text, prefix='', chapter=None):
    for key, path in CODE_PATHS.items():
        target = quote(prefix + '代码/' + path.as_posix(), safe='/')
        text = text.replace('{{code-link:' + key + '}}', f'[完整代码：{path.name}]({target})')
        dependency_target = '#algorithm-' + key
        target_chapter = BY_KEY[key]['chapter']
        if chapter is not None and target_chapter != chapter:
            chapter_number = list(GROUPS).index(target_chapter) + 1
            dependency_target = quote(f'{chapter_number:02d}_{target_chapter}.md', safe='') + dependency_target
        text = text.replace('{{dependency-link:' + key + '}}',
                            f'[查看 {NUMBERS[key]}「{BY_KEY[key]["title"]}」]({dependency_target})')
    text = render_code_paths(text)
    if re.search(r'\{\{(?:code|code-link|section|dependency-link):', text):
        raise ValueError('Unresolved documentation reference')
    return text


def algorithm_body(key):
    return render_code_paths(HELPERS[key] if key in HELPERS else BY_KEY[key]['code'])


def algorithm_blocks(key, emitted=None, active=None):
    if emitted is None:
        emitted = set()
    if active is None:
        active = set()
    if key in active:
        raise ValueError(f'Cyclic dependency: {key}')
    if key in emitted:
        return ''
    active.add(key)
    result = ''
    for dep, _ in DEPS.get(key, []):
        result += algorithm_blocks(dep, emitted, active)
        if dep in CODE_PATHS:
            result += f'// 依赖手册 {NUMBERS[dep]}「{BY_KEY[dep]["title"]}」，以上已展开其实现。\n'
    result += algorithm_body(key) + '\n\n'
    active.remove(key)
    emitted.add(key)
    return result


def standalone_source(key):
    if key == 'starter':
        return STARTER + '\n'
    body = algorithm_blocks(key)
    if 'solve' in BY_KEY[key]:
        main = 'signed main()' + ENTRYPOINT.split('signed main()', 1)[1]
        return PRELUDE.rstrip() + '\n\n' + body + BY_KEY[key]['solve'] + '\n\n' + main + '\n'
    return PRELUDE.rstrip() + '\n\n' + body + 'void solve()' + ENTRYPOINT + '\n'


def verification_header():
    text = '// 自动生成，仅供测试；写题使用独立 cpp。\n#pragma once\n' + PRELUDE
    emitted = set()
    def append(key):
        nonlocal text
        if key in emitted:
            return
        imports = ''
        for dep, names in DEPS.get(key, []):
            append(dep)
            imports += ''.join(f'using cp_{dep}::{name};\n' for name in names)
        body = algorithm_body(key)
        if key in CODE_PATHS:
            source = (OUT/'代码'/CODE_PATHS[key]).read_text(encoding='utf-8')
            if body not in source:
                raise ValueError(f'Test adapter differs from actual program: {key}')
        text += f'\nnamespace cp_{key} {{\n' + imports + body + '\n}\n'
        emitted.add(key)
    for key in BY_KEY:
        if key != 'starter':
            append(key)
    # Existing segment-tree tests use the public global class names.
    text += '\nusing cp_lazyseg::segmenttree;\nusing cp_segtree::SegTree;\n'
    return text


def contents(i, sections):
    lines = []
    for j, (title, methods) in enumerate(sections.items(), 1):
        grouped = 'section' in methods[0]
        target = 'section-' + methods[0]['key'] if grouped else 'algorithm-' + methods[0]['key']
        lines.append(f'- [{i}.{j} {title}](#{target})')
        if grouped:
            for e in methods:
                lines.append(f'  - [{NUMBERS[e["key"]]} {e["title"]}](#algorithm-{e["key"]})')
    return '\n'.join(lines) + '\n'


def render_method(e, level):
    key = e['key']
    lines = [f'<a id="algorithm-{key}"></a>\n', f'{"#"*level} {NUMBERS[key]} {e["title"]}\n', '{{code-link:' + key + '}}\n']
    if e.get('intro'):
        lines.append(e['intro'])
    for field, label in [('use','用途与条件'), ('cost','复杂度'), ('pitfalls','易错点')]:
        lines.append(f'**{label}**：{e[field]}\n')
    deps = [dep for dep, _ in DEPS.get(key, []) if dep in CODE_PATHS]
    if deps:
        lines.append('**依赖**：' + '、'.join('{{dependency-link:' + d + '}}' for d in deps) + '；完整代码已展开依赖，可单独使用。\n')
    helper_code = '\n\n'.join(HELPERS[d] for d, _ in DEPS.get(key, []) if d in HELPERS)
    snippet = (helper_code + '\n\n' if helper_code else '') + e['code']
    lines.extend([f'```cpp\n{snippet}\n```\n', f'**调用与说明**：{e["example"]}\n'])
    for field in ('code_after', 'after'):
        if e.get(field):
            # Explanatory headings remain below the method heading.
            lines.append(re.sub(r'^#{3,4} ', '#'*(level+1)+' ', e[field], flags=re.M))
    return '\n'.join(lines)


def remove_obsolete(expected):
    """Remove only paths previously generated by this project, within code root."""
    code_root = (OUT/'代码').resolve()
    manifest = OUT/'代码文件清单.json'
    previous = json.loads(manifest.read_text(encoding='utf-8')) if manifest.exists() else []
    # One-time migration list is explicit; never enumerate arbitrary files for deletion.
    legacy = ROOT/'legacy_code_paths.json'
    if legacy.exists():
        previous += json.loads(legacy.read_text(encoding='utf-8'))
    for relative in set(previous) - set(expected):
        path = (code_root/relative).resolve()
        if not path.is_relative_to(code_root) or path.suffix != '.cpp':
            raise ValueError(f'Unsafe obsolete path: {relative}')
        path.unlink(missing_ok=True)
    manifest.write_text(json.dumps(expected, ensure_ascii=False, indent=2)+'\n', encoding='utf-8')


def main():
    for folder in ('代码', '章节', '测试'):
        (OUT/folder).mkdir(parents=True, exist_ok=True)
    full = ['# ICPC / CCPC 算法模板手册\n', '> C++17 · 个人可编辑版 · 2026-10-11\n',
            '## 阅读约定\n',
            '- 普通数组、图和树顶点使用 **1..n**，数组查询默认闭区间 **[l,r]**；字符串、多项式系数等例外按各节说明。\n'
            '- 起手式保留 `i64`、`i128`、`u64`；`// #define int long long` 默认注释。使用前检查数值范围、前置条件和复杂度。\n'
            '- 同一问题的不同方法放在同一节的不同小标题下，每个方法各有独立 `.cpp`。背包、博弈小标题注明不同问题变体。\n'
            '- 每份程序包含完整起手式和必要依赖，在 `solve()` 中写题后单独编译；手册片段省略起手式及已列出的依赖。\n'
            '- 几何片段共用点、叉积等基础定义；完整文件已包含这些定义。文件名为对应手册编号和算法名，目录保留章节大标题。\n',
            '## 目录\n']
    for i, (chapter, sections) in enumerate(GROUPS.items(), 1):
        full.append(f'- [{i}. {chapter}](#chapter-{i})\n' + '\n'.join('  '+s for s in contents(i, sections).splitlines())+'\n')
    for i, (chapter, sections) in enumerate(GROUPS.items(), 1):
        header = f'<a id="chapter-{i}"></a>\n\n# {i}. {chapter}\n'
        body = []
        for j, (title, methods) in enumerate(sections.items(), 1):
            grouped = 'section' in methods[0]
            if grouped:
                body.append(f'<a id="section-{methods[0]["key"]}"></a>\n\n## {i}.{j} {title}\n')
            for e in methods:
                body.append(render_method(e, 3 if grouped else 2))
        content = '\n'.join(body)
        full.append(header+'\n'+content)
        chapter_text = header+'\n## 本章算法目录\n\n'+contents(i,sections)+'\n'+content
        (OUT/'章节'/f'{i:02d}_{chapter}.md').write_text(render_code_links(chapter_text,'../',chapter=chapter),encoding='utf-8')
    full.append((ROOT/'appendix.md').read_text(encoding='utf-8'))
    (OUT/'算法模板手册.md').write_text(render_code_links('\n'.join(full)),encoding='utf-8')
    for key, path in CODE_PATHS.items():
        dest = OUT/'代码'/path
        dest.parent.mkdir(parents=True, exist_ok=True)
        dest.write_text(standalone_source(key),encoding='utf-8')
    (OUT/'测试/algorithm_adapters.hpp').write_text(verification_header(),encoding='utf-8')
    (OUT/'测试/segmenttree_adapter.hpp').write_text('// 自动生成，仅供线段树测试。\n#pragma once\n'+algorithm_body('lazyseg')+'\n'+algorithm_body('segtree')+'\n',encoding='utf-8')
    remove_obsolete([p.as_posix() for p in CODE_PATHS.values()])
    print(f'Generated {len(CODE_PATHS)} standalone programs and {len(GROUPS)} chapters.')


if __name__ == '__main__':
    main()
