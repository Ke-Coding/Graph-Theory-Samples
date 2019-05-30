/*

无向图和有向图的区别是什么？
虽然都有反向弧，但是对于有向图，反向弧是用来反悔的，所以反向弧初始容量是0，每次正向弧选多少就给它增加多少的反悔空间。
换句话说也就是说每一对反向弧的【净流量】必须是沿初始那个有向位置的！（反向弧再反悔也不能反悔到反向）


而无向图，反向图初始容量就和正弧相同，也就是说那条路上的净流量可以是【双向】的！
 



dinic 普通分层版 732ms
【实测，对于本题，不论正分层还是逆分层，速度：下标版链式前向星 快于 指针版链式前向星 快于 vector图】

 
https://www.luogu.org/problemnew/show/P4001

*/

#include <cstdio>
#include <cstring>
#include <limits>

#define LL long long
#define ULL unsigned LL

#define _BS 1048576
char _bf[_BS];
char *__p1=_bf,*__p2=_bf;
#define _IO char *_p1=__p1,*_p2=__p2;
#define _OI __p1=_p1,__p2=_p2;

#ifdef _KEVIN
#define GC getchar()
#else
#define GC (_p1==_p2&&(_p2=(_p1=_bf)+fread(_bf,1,_BS,stdin),_p1==_p2)?EOF:*_p1++)
#endif

#define PC putchar
#define _Q_(x) {register char _c=GC,_v=1;for(x=0;_c<48||_c>57;_c=GC)if(_c==45)_v=-1;
#define _H_(x) for(;_c>=48&&_c<=57;x=(x<<1)+(x<<3)+_c-48,_c=GC);if(_v==-1)x=-x;}
#define sc(x) _Q_(x)_H_(x)
#define se(x) _Q_(x)else if(_c==EOF)return 0;_H_(x)
#define _G1(_1) int _1;sc(_1)
#define _G2(_1,_2) int _1,_2;sc(_1)sc(_2)
#define _G3(_1,_2,_3) int _1,_2,_3;sc(_1)sc(_2)sc(_3)
#define _gG(_1,_2,_3,_get, ...) _get
#define get(...) _gG(__VA_ARGS__,_G3,_G2,_G1, ...)(__VA_ARGS__)
#define _F0N(i,n) for(auto i=0;i<(n);++i)
#define _FLR(i,l,r) for(auto i=(l);i<(r);++i)
#define _gF(_1, _2, _3, _F, ...) _F
#define F(...) _gF(__VA_ARGS__,_FLR,_F0N, ...)(__VA_ARGS__)
#define _FD0(i,n) for(auto i=0;i<=(n);++i)
#define _FDL(i,l,r) for(auto i=(l);i<=(r);++i)
#define _gFD(_1, _2, _3, _FD, ...) _FD
#define FD(...) _gFD(__VA_ARGS__,_FDL,_FD0, ...)(__VA_ARGS__)
#define FRC(R,C) for(int r=0;r<R;++r)for(int c=0;c<C;++c)


template<class T>
void UPRT(const T _){if(_>=10)UPRT(_/10);PC(_%10+48);}
#define UPL(_) UPRT(_),PC(10)
template<class T>
void PRT(const T _){if(_<0){PC(45),UPRT<ULL>(-(ULL)_);return;}if(_>=10)PRT(_/10);PC(_%10+48);}
#define PL(_) PRT(_),PC(10)

#define MAX(a, b) ((a)>(b)?(a):(b))
#define MIN(a, b) ((a)<(b)?(a):(b))


#ifdef _KEVIN
constexpr int MN = 1e2+3, MV = MN*MN, ME = MV << 6;
#else
constexpr int MN = 1e3+3, MV = MN*MN, ME = MV << 6;
#endif


template <const bool directed, typename cint>
class Dinic
{
private:

//#define CINT_MAX LLONG_MAX
	static constexpr auto CINT_MAX = std::numeric_limits<cint>::max();
	struct Ed
	{
		int ne, v;
		cint c;
	} ed[ME];
	int head[MV], cur[MV], tot;
	int V, S, T;
	
	int de[MV];
    bool bfs()
    {
        static int q[MV];
        int hd = 0, tl = 0;

        memset(de, 0, sizeof(*de) * (V+2));
        de[q[tl++]=S] = 1;
        while (hd != tl)
        {
            const int u = q[hd++];
            for (int i=head[u]; i; i=ed[i].ne)
            {
                const int v = ed[i].v;
                if (ed[i].c && !de[v])
                {
                    de[q[tl++]=v] = de[u] + 1;
                    if (v == T)
                        return true;
                }
            }
        }
        
        return false;
    }
	
	cint dfs(const int u, cint in)	// 进dfs时，in一定>0
	{
		if (u == T)
			return in;
		cint out = 0, now;
		/* 【当前弧优化】：跳过一定无法再增广的边。
			什么时候可以认为一条边一定无法再增广了？就是曾经有>0的in流量放在了它的起点，但在经过增广后in流量并没有用完。这就说明它不可能再增广更多了。
			因此for循环每进行一次（相当于榨干了一条边的增广潜力），我们就可以更新一次cur[u]，直到in用完或者已经遍历完u的一切出边了。
			所以要注意最后一次循环到底是怎么退出的：
			1. (in==0 && i!=0) || (in==0 && i==0)：
				假设最后一次循环内这条边叫ed[i0]。ed[i0].ne==i1。那么ed[i0]可能是仍然能增广的（这次是因为in没了才没增广成功的，如果再给一些in，可能还可以增广）
				那么退出for之后cur[u]应该是多少？显然应该是i0（因为ed[i0]可能还有潜力，跳过他就可能错过了，就失去了优化效果。）。
				所以我们不能把for循环写成下面三种样子（他们是互相等价的）：（写成这样的话退出循环后cur[u]就会是i1了）
					a) for (int i=cur[u]; i&&in; cur[u]=i=ed[i].ne)
					b) for (int &i=cur[u]; i&&in; i=ed[i].ne)（和a写法完全等价）
					c) for (int i=cur[u]; (cur[u]=i) && in; i=ed[i].ne)（和a写法相比只是在进入循环时多了一次没用的赋值）
				但是这样可以：（刚好利用了&&的短路性质，使得for循环因为in==0退出时，cur[u]不会被更新成i1）
					d) for (int i=cur[u]; in && (cur[u]=i); i=ed[i].ne)
				或者我们把in==0时跳出的这句话给移出for的第二部分，移到for循环体内，那还是可以用以上三种写法的（特别是引用，这个最方便）。
			2. in!=0 && i==0：
				只有这种情况才可以把cur[u]更新成i2，且此时i2刚好是0。
			
			所以到底怎么写当前弧优化呢？上面有一种利用&&短路性质的写法。除了这种，更常见的写法是在for循环体里面写。
			那么写在哪？只要写在if外面都能起到及时更新而又不会在in==0时误更新的效果（不用担心写在for循环体最后会导致更新不及时。因为中间的递归过程里不可能再经过u这个点（因为一直是按dep的严格增序dfs的））
			而最好不要写在if里面。不能说这条边没进if就不更新了。实际上如果这条边连再向下dfs都不行的话，更说明他无法再增广了。
			
			但是这些在for里面写的写法有极为轻微的性能降低，因为他们绝对不会在循环结束的那次进行更新。
			所以如果循环结束时in!=0而i==0，他们就没更新，而实际上这种情况是可以把cur[u]更新成i（也就是0）的。因为in流量还有剩余，说明u发出的所有边都一定不能再增广了
			
			不过可以通过在最后炸点的时候把in!=0的点也炸掉来弥补这个微弱的性能降低 
		*/
		for (int i=cur[u]; in&&(cur[u]=i); i=ed[i].ne)		// 【判in优化】：剪枝
			if (ed[i].c>0 && de[ed[i].v]==de[u]+1 && (now=dfs(ed[i].v, MIN(in, ed[i].c))))	// 【逆分层】前往的v结点深度更小
				in -= now, out += now, ed[i].c -= now, ed[i^1].c += now;
		
		// 因为进函数时in必>0，所以退出函数前若in==0则out必!=0，而in!=0时out可能为0（没增广或者增广了0）可能不为0，out==0时in可能为0（in耗尽）也可能不为0（没耗尽）
		// 所以if的两个条件并不是包含关系，因此不用担心某个条件是多余的
		if (in || !out) // in不等于0就退出for，说明u的所有出边都无法再增广；out==0则显然是增广失败的标志。
			de[u] = -1;	// 【炸点优化】：此路不通，剪掉盲枝
		return out;
	}
	
public:
	
	void init(const int v)
	{
		memset(head, 0, sizeof(*head) * (V+2));	// 注意是清V+2个，不是清v+2个。这样第一次就只会清2个
		V = v;
		tot = 1;	// 不是2，如果tot是2那第一条边就是3了... 要让第一条边是非0偶数才行
	}
	
	void edd(const int u, const int v, const cint c = CINT_MAX)	// 不填容量就是加无穷大容量
	{
		ed[++tot].ne=head[u], ed[tot].v=v, ed[tot].c=c, head[u]=tot;
		ed[++tot].ne=head[v], ed[tot].v=u, ed[tot].c=directed ? 0:c, head[v]=tot;	// 有向:ed[tot].c=0; 无向:ed[tot].c=c
	}
	
	cint max_flow(const int s, const int t)
	{
		cint sum = 0;
		S = s, T = t;
		while (bfs())
		{
			memcpy(cur, head, sizeof(*head) * (V+2));
			sum += dfs(s, CINT_MAX);
		}
		return sum;
	}
};

Dinic<false, int> mf;


int id[MN][MN];
int main()
{
	_IO
	get(R, C)
	const int V = R*C, S = 1, T = S+V-1;
	mf.init(V);
	
	int w;
	int tot = S-1;
	F(r, R)
	{
		id[r][0] = ++tot;
		F(c, 1, C)
		{
			id[r][c] = ++tot;
			sc(w)
			mf.edd(tot-1, tot, w);
		}
	}
	
	F(r, 1, R)
	{
		F(c, C)
		{
			sc(w)
			mf.edd(id[r][c]-C, id[r][c], w);
		}
	}
	
	F(r, 1, R)
	{
		F(c, 1, C)
		{
			sc(w)
			mf.edd(id[r][c]-C-1, id[r][c], w);
		}
	}
	
	PL(mf.max_flow(S, T));
	
	return 0;
}
