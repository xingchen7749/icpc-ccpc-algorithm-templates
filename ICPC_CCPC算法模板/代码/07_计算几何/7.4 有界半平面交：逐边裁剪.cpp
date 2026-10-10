#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
// #define int long long

using ld=long double;const ld EPS=1e-12L;
struct P{ld x,y;
    // 返回当前二维向量与 b 的逐坐标和，不修改原向量；时间 O(1)。
    P operator+(P b)const{return {x+b.x,y+b.y};}
    // 返回当前二维向量减去 b 的结果，可表示从 b 指向当前点的位移；时间 O(1)。
    P operator-(P b)const{return {x-b.x,y-b.y};}
    // 返回当前向量乘标量 k 的结果，不修改原向量；时间 O(1)。
    P operator*(ld k)const{return {x*k,y*k};}};
// 返回二维向量 a、b 的叉积，正负表示旋转方向；浮点结果需结合 EPS 判断，O(1)。
ld cross(P a,P b){return a.x*b.y-a.y*b.x;}
// 返回二维向量 a、b 的点积，可用于投影和长度平方计算；时间 O(1)。
ld dot(P a,P b){return a.x*b.x+a.y*b.y;}
// 返回直线 p+t*v 与 q+s*w 的唯一交点；v、w 必须是非零方向向量。
// 在 EPS 意义下平行或重合时返回 nullopt，不区分这两种情况；时间 O(1)。
optional<P> line_intersection(P p,P v,P q,P w){
    ld d=cross(v,w);if(fabsl(d)<EPS)return nullopt;
    return p+v*(cross(q-p,w)/d);
}
// 求圆心 a、半径 r 与圆心 b、半径 s 两圆的交点，要求半径非负。
// 返回 {数量,交点列表}：0/1/2 为有限交点数，-1 表示重合且有无穷交点。
// 按 EPS 判定，重合的零半径圆按一个交点处理；时间和空间 O(1)。
pair<int,vector<P>> circle_intersection(P a,ld r,P b,ld s){
    assert(r>=0&&s>=0);P v=b-a;ld d=sqrtl(dot(v,v));
    if(d<EPS){
        if(fabsl(r-s)>EPS)return {0,{}};
        if(r<EPS)return {1,{a}};return {-1,{}};
    }
    if(d>r+s+EPS||d<fabsl(r-s)-EPS)return {0,{}};
    ld x=(r*r-s*s+d*d)/(2*d),h2=r*r-x*x;
    if(h2<-EPS)return {0,{}};P base=a+v*(x/d);
    if(h2<=EPS)return {1,{base}};
    P off=P{-v.y,v.x}*(sqrtl(max((ld)0,h2))/d);
    return {2,{base+off,base-off}};
}

// Dependency: P, cross, EPS.
// 用有向直线 a→b 裁剪凸多边形 poly，保留其左侧及边界，返回新的循环顶点列表。
// 要求 a!=b，顶点按边界顺序排列且不带占位；空结果表示在容差意义下无交集。
// 不修改输入，每次耗时 O(顶点数)；连续裁剪需从保证覆盖解集的包围框开始。
vector<P> clip_halfplane(const vector<P>&poly,P a,P b){
    vector<P>out;int n=poly.size();P v=b-a;
    assert(dot(v,v)>0);
    for(int i=0;i<n;++i){
        P p=poly[i],q=poly[(i+1)%n];ld dp=cross(v,p-a),dq=cross(v,q-a);
        bool ip=dp>=-EPS,iq=dq>=-EPS;
        if(ip)out.push_back(p);
        if(ip!=iq){
            // Classification uses EPS; clamp near-boundary interpolation.
            ld t=dp/(dp-dq);t=max((ld)0,min((ld)1,t));
            out.push_back(p+(q-p)*t);
        }
    }return out;
}

void solve() {
    // Read one test case. Arrays and vertices use 1..n.
}
signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T = 1;
    cin >> T; // Remove for a single test case.
    while (T--) solve();
}
