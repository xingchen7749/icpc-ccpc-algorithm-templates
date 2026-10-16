"""Compile the standalone binary trie and compare it with a multiset oracle."""
from pathlib import Path
import subprocess
import tempfile

SOURCE = Path(__file__).resolve().parents[1] / '代码/02_数据结构/2.10 01 Trie：二进制字典树.cpp'
TEST = r'''
int main() {
    BinaryTrie edge;
    u64 high = 1ULL << 63, full = ~0ULL;
    edge.insert(0);
    edge.insert(high);
    edge.insert(full);
    edge.insert(full);
    assert(edge.max_xor(0) == full);
    assert(edge.erase(full) && edge.count(full) == 1);
    assert(edge.erase(full) && edge.max_xor(0) == high);
    assert(!edge.erase(full));
    assert(edge.erase(high) && edge.erase(0) && edge.size() == 0);
    edge.insert(full);
    assert(edge.max_xor(0) == full);
    edge.clear();
    assert(edge.size() == 0 && edge.count(full) == 0);

    mt19937_64 rng(20261016);
    for (int bits : {0, 1, 5, 30, 62, 63}) {
        BinaryTrie trie(bits);
        multiset<u64> a;
        u64 mask = bits == 63 ? ~0ULL : (1ULL << (bits + 1)) - 1;
        for (int step = 0; step < 12000; step++) {
            u64 x = rng() & mask;
            int op = rng() % 6;
            if (op == 0 || op == 1) {
                trie.insert(x);
                a.insert(x);
            }
            else if (op == 2) {
                if (!a.empty() && (rng() & 1)) x = *a.begin();
                auto it = a.find(x);
                assert(trie.erase(x) == (it != a.end()));
                if (it != a.end()) a.erase(it);
            }
            else if (op == 3) {
                assert(trie.count(x) == (int)a.count(x));
            }
            else if (op == 4 && !a.empty()) {
                u64 ans = 0;
                for (u64 y : a) ans = max(ans, x ^ y);
                assert(trie.max_xor(x) == ans);
            }
            else if (op == 5 && step % 101 == 0) {
                trie.clear();
                a.clear();
            }
            assert(trie.size() == (int)a.size());
        }
        for (u64 x : a) assert(trie.erase(x));
        assert(trie.size() == 0);
        trie.insert(mask);
        assert(trie.max_xor(0) == mask);
    }
    cout << "BinaryTrie: boundaries and 72000 randomized operations passed.\n";
}
'''

with tempfile.TemporaryDirectory(prefix='binary_trie_') as directory:
    folder = Path(directory)
    original = SOURCE.read_text(encoding='utf-8')
    (folder / 'standalone.cpp').write_text(original, encoding='utf-8')
    (folder / 'test.cpp').write_text(
        original.replace('signed main()', 'void template_main()') + TEST,
        encoding='utf-8',
    )
    for name in ('standalone', 'test'):
        subprocess.run(['g++', '-std=c++17', '-O2', '-Wall', '-Wextra',
                        str(folder / (name + '.cpp')), '-o', str(folder / (name + '.exe'))], check=True)
    subprocess.run([str(folder / 'test.exe')], check=True)
