#include <bits/stdc++.h>
using namespace std;
#ifdef WIDE_SEG
#define int long long
#endif
#include "segmenttree_adapter.hpp"
signed main() {
    mt19937 rng(42);
    for (int n : {1, 2, 7, 30}) {
        vector<int> a(n+1), b(n+1);
        segmenttree st(a);
        SegTree plain(b);
        for (int k=0;k<1000;++k) {
            int l=rng()%n+1, r=rng()%n+1;
            if(l>r) swap(l,r);
            int v=(int)(rng()%201)-100;
#ifdef WIDE_SEG
            v *= 100000000LL;
#endif
            if(k%3==0) {
                st.add(l,r,v);
                for(int i=l;i<=r;++i) a[i]+=v;
                plain.add(l,v); b[l]+=v;
            } else if(k%3==1) {
                st.set(l,v); a[l]=v;
                plain.set(l,v); b[l]=v;
            }
            for(int x=1;x<=n;++x) for(int y=x;y<=n;++y) {
                assert(st.query(x,y)==accumulate(a.begin()+x,a.begin()+y+1,0LL));
                assert(plain.query(x,y)==accumulate(b.begin()+x,b.begin()+y+1,0LL));
            }
        }
    }
    cout << "PASS both segment trees\n";
}
