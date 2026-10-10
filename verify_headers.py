"""Verify self-contained single-file contest programs (historical script name)."""
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
import json
import re
import subprocess
import tempfile

from build_handbook import CODE_PATHS
from handbook_data import ENTRIES

ROOT = Path(__file__).resolve().parent
CODE = ROOT / 'ICPC_CCPC算法模板/代码'
ALGORITHMS = [key for key in CODE_PATHS if key != 'starter']


def run(args, **kwargs):
    result = subprocess.run(args, text=True, capture_output=True,
                            encoding='utf-8', errors='replace', timeout=120, **kwargs)
    if result.returncode:
        raise RuntimeError(result.stdout + result.stderr)


def check_program(key):
    relative_path = CODE_PATHS[key]
    filename = relative_path.name
    try:
        source = (CODE / relative_path).read_text(encoding='utf-8')
        if re.search(r'^\s*#\s*include\s*"', source, re.M):
            raise RuntimeError('Standalone source contains a local include')
        if re.search(r'CP_FILE_|CP_ALGORITHM_|__INCLUDE_LEVEL__|namespace cp_', source):
            raise RuntimeError('Obsolete include compatibility wrapper remains')
        if key != 'starter' and (CODE / f'{key}.hpp').exists():
            raise RuntimeError('Obsolete algorithm header still exists')
        with tempfile.TemporaryDirectory(prefix='contest_check_') as directory:
            isolated = Path(directory)
            (isolated / filename).write_text(source, encoding='utf-8')
            executable = isolated / 'program.exe'
            run(['g++', '-std=c++17', filename, '-o', str(executable)], cwd=isolated)
            run([str(executable)], input='0\n', cwd=isolated)
        return {'case': filename, 'ok': True}
    except (OSError, RuntimeError, subprocess.TimeoutExpired) as error:
        return {'case': filename, 'ok': False, 'error': str(error)}


if __name__ == '__main__':
    results = []
    with ThreadPoolExecutor(max_workers=4) as pool:
        for result in pool.map(check_program, ['starter'] + ALGORITHMS):
            results.append(result)
            if not result['ok']:
                print(result, flush=True)
            if len(results) % 10 == 0:
                print(f'Checked {len(results)}/{len(CODE_PATHS)} standalone programs', flush=True)
    (ROOT / 'tmp').mkdir(exist_ok=True)
    (ROOT / 'tmp/header_checks.json').write_text(
        json.dumps(results, ensure_ascii=False, indent=2), encoding='utf-8')
    failed = [r for r in results if not r['ok']]
    for result in failed:
        print(result['case'], result['error'])
    print(f'{len(results)-len(failed)}/{len(results)} checks passed')
    raise SystemExit(bool(failed))
