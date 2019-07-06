#include <cstdio>
#include <cstring>
#include <cmath>
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
#define FD(i, l, r) for (int i=l; i<=r; ++i)

template<class T>
void UPRT(const T _){if(_>=10)UPRT(_/10);PC(_%10+48);}
#define UPL(_) UPRT(_),PC(10)
template<class T>
void PRT(const T _){if(_<0){PC(45),UPRT<ULL>(-(ULL)_);return;}if(_>=10)PRT(_/10);PC(_%10+48);}
#define PL(_) PRT(_),PC(10)


#define SW(a, b) ({auto __tp=a; a=b; b=__tp;})
#define MIN(a, b) ((a)<(b)?(a):(b))

#ifdef _KEVIN
constexpr int MV(5e2+7), ME(1e5+7);
#else
constexpr int MV(5e3+7), ME(1e5+7);
#endif


template <typename cint, typename wint>
class MCMF
{
public:
//#define CINT_MAX LLONG_MAX
	static constexpr cint CINT_MAX = std::numeric_limits<cint>::max();
	static constexpr wint WINF = (wint)0x3f3f3f3f3f3f3f3fLL;

private:
	struct Ed
	{
		int v, ne;
		cint c;
		wint w;
	} ed[ME];
	int head[MV], tot;
	int V, S, T;

	wint ds[MV];
	int pre[MV];
#define SLF_SW if (tl-hd>1 && ds[q[hd]]>ds[q[tl-1]]) SW(q[hd], q[tl-1])
	bool spfa()
	{
		static int q[MV*MV];
		static bool inq[MV];
		int hd = 0, tl = 0, u, v;
		memset(ds, WINF, sizeof(*ds) * (V+2)), ds[S] = 0;
		
		inq[q[tl++]=S] = true;
		while (hd != tl)
		{
			inq[u=q[hd++]] = false;
			SLF_SW;
			for (int i=head[u]; i; i=ed[i].ne)
			{
				v = ed[i].v;
				if (ed[i].c && ds[v] > ds[u] + ed[i].w)
				{
					ds[v] = ds[u] + ed[i].w;
					pre[v] = i;
					if (!inq[v])
					{
						inq[q[tl++]=v] = true;
						SLF_SW;
					}
				}
			}
		}

		return ds[T] != WINF;
	}

	void aug()
	{
		int ei;
		cint f = CINT_MAX;
		for (int u=T; u!=S; u=ed[ei^1].v)
		{
			ei = pre[u];
			if (f > ed[ei].c)
				f = ed[ei].c;
		}
		mc += ds[T] * f, mf += f;
		
		for (int u=T; u!=S; u=ed[ei^1].v)
		{
			ei = pre[u];
			ed[ei].c -= f, ed[ei^1].c += f;
		}
	}

public:
	void init(const int v)
	{
		memset(head, 0, sizeof(*head) * (V+2));
		V = v, tot = 1;
	}

	void edd(const int u, const int v, const cint c, const wint w)
	{
		ed[++tot].v=v, ed[tot].c=c, ed[tot].w=w, ed[tot].ne=head[u], head[u]=tot;
		ed[++tot].v=u, ed[tot].c=0, ed[tot].w=-w, ed[tot].ne=head[v], head[v]=tot;
	}

	wint mc;
	cint mf;
	void mcmf(const int s, const int t)
	{
		S = s, T = t;
		mc = mf = 0;
		while (spfa())
			aug();
	}
};


MCMF<int, int> mf;

int main()
{
	_IO

	get(V, E)
	get(S, T)
	mf.init(V);

	int u, v;
	int c, w;
	while (E--)
	{
		sc(u)sc(v)sc(c)sc(w)
		mf.edd(u, v, c, w); 
	}

	mf.mcmf(S, T);
	PRT(mf.mf), PC(32), PRT(mf.mc);

	return 0;
}
