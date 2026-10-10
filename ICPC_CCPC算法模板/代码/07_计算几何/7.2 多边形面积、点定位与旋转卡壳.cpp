#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
// #define int long long

struct Point {
    i64 x,y;
    // 按 (x,y) 字典序比较当前点和 b，供严格排序使用；不使用浮点容差，O(1)。
    bool operator<(Point b)const{return tie(x,y)<tie(b.x,b.y);}
    // 判断当前点与 b 的两个整数坐标是否完全相等，供点集去重使用，O(1)。
    bool operator==(Point b)const{return x==b.x&&y==b.y;}
};
// 返回向量 AB 与 AC 的叉积，即三点构成三角形的两倍有向面积。
// 正值表示 a→b→c 逆时针，0 表示共线；坐标绝对值要求不超过 10^18，O(1)。
i128 cross(Point a,Point b,Point c){
    return (i128(b.x)-a.x)*(i128(c.y)-a.y)-(i128(b.y)-a.y)*(i128(c.x)-a.x);
}
// 返回 x 的符号：负数为 -1，零为 0，正数为 1；时间 O(1)。
int sign(i128 x){return (x>0)-(x<0);}
// 判断点 p 是否在线段 ab 上，包含端点，也支持 a=b 的退化线段。
// 使用精确叉积和坐标范围判断，坐标满足整数几何范围约定；时间 O(1)。
bool on_segment(Point p,Point a,Point b){
    return cross(a,b,p)==0&&min(a.x,b.x)<=p.x&&p.x<=max(a.x,b.x)
        &&min(a.y,b.y)<=p.y&&p.y<=max(a.y,b.y);
}
// 判断闭线段 ab 与 cd 是否有交点，包含端点接触、共线重叠和退化点。
// 使用整数精确判断，坐标绝对值不超过 10^18；时间 O(1)。
bool intersect(Point a,Point b,Point c,Point d){
    int x=sign(cross(a,b,c)),y=sign(cross(a,b,d));
    int z=sign(cross(c,d,a)),w=sign(cross(c,d,b));
    if(!x&&on_segment(c,a,b))return true;if(!y&&on_segment(d,a,b))return true;
    if(!z&&on_segment(a,c,d))return true;if(!w&&on_segment(b,c,d))return true;
    return x*y<0&&z*w<0;
}
// 返回点集 p 的逆时针凸包顶点列表，不带占位、不重复首点，删除边上中间共线点。
// 输入可无序、有重复，不修改调用者的点集；全共线时只留端点，时间 O(n log n)。
vector<Point> convex_hull(vector<Point>p){
    sort(p.begin(),p.end());p.erase(unique(p.begin(),p.end()),p.end());
    int n=p.size();if(n<=1)return p;vector<Point>h(2*n);int k=0;
    for(auto v:p){while(k>=2&&cross(h[k-2],h[k-1],v)<=0)--k;h[k++]=v;}
    for(int i=n-2,t=k+1;i>=0;--i){
        while(k>=t&&cross(h[k-2],h[k-1],p[i])<=0)--k;h[k++]=p[i];
    }
    h.resize(k-1);return h;
}

// Dependency: Point, cross, on_segment.
// 返回按边界顺序排列的多边形顶点 p 的两倍有向面积，逆时针为正、顺时针为负。
// p 是无占位循环列表，不需重复首点；要求叉积累加不溢出 i128，时间 O(n)。
i128 area2(const vector<Point>&p){
    i128 a=0;int n=p.size();for(int i=0;i<n;++i){auto q=p[(i+1)%n];
        a+=i128(p[i].x)*q.y-i128(p[i].y)*q.x;}return a;
}
// 判断点 q 与简单多边形 p 的位置关系：0 为外部、1 为边界、2 为内部。
// p 是按边界顺序排列的无占位循环列表，顺逆时针均可；时间 O(n)。
int inside(const vector<Point>&p,Point q){
    int wn=0,n=p.size();for(int i=0;i<n;++i){Point a=p[i],b=p[(i+1)%n];
        if(on_segment(q,a,b))return 1;
        if(a.y<=q.y&&b.y>q.y&&cross(a,b,q)>0)++wn;
        if(a.y>q.y&&b.y<=q.y&&cross(a,b,q)<0)--wn;
    }return wn?2:0;
}
// 返回整数点 a、b 的欧氏距离平方，避免开方；坐标绝对值不超过 10^18，时间 O(1)。
i128 dist2(Point a,Point b){i128 x=i128(a.x)-b.x,y=i128(a.y)-b.y;return x*x+y*y;}
// 用旋转卡壳返回凸多边形最远两点距离的平方；不足两个点返回 0。
// p 必须逆时针排列且无边上中间共线点，可直接使用 convex_hull 的结果；时间 O(n)。
i128 diameter2(const vector<Point>&p){
    int n=p.size();if(n<2)return 0;if(n==2)return dist2(p[0],p[1]);
    i128 ans=0;int j=1;
    for(int i=0;i<n;++i){int ni=(i+1)%n;
        while(cross(p[i],p[ni],p[(j+1)%n])>cross(p[i],p[ni],p[j]))j=(j+1)%n;
        ans=max({ans,dist2(p[i],p[j]),dist2(p[ni],p[j])});
    }return ans;
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
