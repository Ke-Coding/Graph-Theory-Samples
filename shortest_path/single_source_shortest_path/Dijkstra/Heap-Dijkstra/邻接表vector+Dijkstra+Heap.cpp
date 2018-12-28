#include <cstdio>
#include <cstring>
#include <vector>


typedef int DistType;
typedef int DistSrcType;

#define sc(x) {register char _c=getchar(),_v=1;for(x=0;_c<48||_c>57;_c=getchar())if(_c==45)_v=-1;for(;_c>=48&&_c<=57;x=(x<<1)+(x<<3)+_c-48,_c=getchar());x*=_v;}
#define se(x) {register char _c=getchar(),_v=1;for(x=0;_c<48||_c>57;_c=getchar())if(_c==45)_v=-1;else if(_c==-1)return 0;for(;_c>=48&&_c<=57;x=(x<<1)+(x<<3)+_c-48,_c=getchar());x*=_v;}
template<typename T>void PRT(const T a){if(a<0){PC(45),PRT(-a);return;}if(a>=10)PRT(a/10);PC(a%10+48);}


// 【注意】 用 long long 时，把 INF 改成 0x3f3f3f3f3f3f3f3fLL，int 时可以用 0x3f3f3f3f 
constexpr DistSrcType INF(0x3f3f3f3f3f3f3f3fLL);
constexpr int MV(100003);
constexpr int ME(1000003);


struct Vertex		// 顶点类，重载了比较函数，用于实现多优先级优先队列
{
	int num;
	DistSrcType dist_src;

	Vertex(void) { }
	Vertex(int num, DistSrcType dist_src) :
		num(num), dist_src(dist_src) { }

	inline bool operator < (const Vertex &o) const
	{
		return dist_src < o.dist_src;
	}
};


template <class Type, int MN>	// 小顶堆
class KPQ
{

#define ChosenType Type& 		// 用原生类型还是自定义类
#define CMP <				// >就变成大顶堆 

#define perc_down(root) int p = root; \
	for (int c, E=cnt>>1; p<=E; p=c) \
	{ \
		c = p<<1; \
		if (c!=cnt && heap[c+1] CMP heap[c]) \
			c++; \
		if (heap[c] CMP tp) \
			heap[p] = heap[c]; \
		else break; \
	} \
	heap[p] = tp
	public:

		Type heap[MN];
		int cnt;

	public:

		KPQ(const ChosenType MIN_SENTRY) : cnt(0)
		{
			*heap = MIN_SENTRY;
		}

		void push(const ChosenType data)
		{
			register int i = ++cnt;
			for (; data CMP heap[i>>1]; i>>=1)
				heap[i] = heap[i>>1];
			heap[i] = data;
		}

		void pop(void)
		{
			const ChosenType tp = heap[cnt--];
			perc_down(1);
		}

		inline const ChosenType top(void)
		{
			return heap[1];
		}

		inline bool empty(void)
		{
			return !cnt;
		}

		inline int size(void)
		{
			return cnt;
		}

		inline void clear(void)
		{
			cnt = 0;
		}

		void build(int n)
		{
			cnt = n;
			// for i:[1,n] scan
			for (int i=1; i<=n; i++)
				sc(heap[i])
				for (int i=n>>1; i>0; i--)
				{
					const Type tp = heap[i];
					perc_down(i);
				}
		}

		void erase(const ChosenType data)
		{
			for (int i=1; i<=cnt; i++)
			{
				if (heap[i] == data)
				{
					heap[i] = heap[cnt--];
					const ChosenType tp = heap[cnt+1];
					perc_down(i);
					return;
				}
			}
		}
};


class AGraph		// std::vector实现邻接表存储的图
{
	public:

		struct Edge
		{
			int dest;
			DistType dist;

			Edge(void) { }
			Edge(int dest, DistType dist) :
				dest(dest), dist(dist) { }
		};
		std::vector<Edge> edge[MV];

		int V, E;
		int src_index;
		int head[MV];
		bool collected[MV];		// 记录Dijkstra中的收录情况
		DistSrcType dist_src[MV];	// 到源点的路径
//		int path[MV];
		KPQ<Vertex, 4*MV> uncollected;

	public:

		AGraph(const Vertex &MIN_SENTRY) : uncollected(MIN_SENTRY) { }

		void clear(void)
		{
			for (int i=0; i<V; ++i)		// 源点编号从0开始，边集数组也是 
				edge[i].clear();
			uncollected.cnt = 0;
		}

		void inputEdges(void)
		{
			int src, dest;
			DistType dist;
			for (int e=0; e<E; ++e)
			{
				sc(src)sc(dest)sc(dist)
				edge[src].emplace_back(Edge(dest, dist));
				edge[dest].emplace_back(Edge(src, dist));
			}
		}

		void initDijkstra()
		{
			memset(collected, false, sizeof(*collected) * (V+1));		// 点都没有被收录
			memset(dist_src, 0x3f, sizeof(*dist_src) * (V+1));		// 到源点的距离都是无穷
//			memset(path, -1, sizeof(*path) * (V+1));			// 初始化路径

			collected[src_index] = true, dist_src[src_index] = 0;		// 收录源点，更新源点到源点的距离(是0)

			for (auto &ed : edge[src_index])
			{
				dist_src[ed.dest] = ed.dist;				// 更新源点直接邻接点的距离
//				path[ed.dest] = src_index;				// 初始化第一批路径
			}

			for (int v=0; v<V; ++v)						// 顶点从 0 开始编号，循环就是 0 -> V-1
			{
				if (v != this->src_index)				// 把未收录的点都入队（一开始只有源点收录了，所以其他的都入队）
					uncollected.push(Vertex(v, dist_src[v]));
			}
		}

		void Dijkstra()
		{
			int u, v;
			DistType dist;
			while (uncollected.cnt)					// 还有未收录顶点
			{
				u = uncollected.heap[1].num;			// 当前最近的顶点
				uncollected.pop();

				if (!collected[u])				// 如果没有收录
				{
					collected[u] = true;			// 收录

					for (auto &ed : edge[u])
					{
						// 对这个顶点的所有邻接点进行松弛操作
						v = ed.dest, dist = ed.dist;
						if (!collected[v])
						{
							if (dist_src[v] > dist_src[u] + dist)
							{
								dist_src[v] = dist_src[u] + dist;
								uncollected.push(Vertex(v, dist_src[v]));	// 被松弛之后（源点距变短）的顶点重新入队（不用删除在优先队列中原来的它，因为这个一定会先出队的，出队就vis了，原来那个就不用管了）
//								path[v] = u;					// 松弛的时候记录路径
							}
						}
					}
				}
			}
		}

		void printMinDist(void)
		{
			for (int v=0; v<V; v++)			// 从0开始，循环就是0 -> V-1
			{
				if (v != this->src_index)	// 无需打印源点到自己的到达情况
				{
					if (dist_src[v] == INF)
						putchar('-'), putchar('1');
					else
						PRT(dist_src[v]);
					putchar(' ');
				}
			}
			putchar('\n');
		}
};

AGraph g(Vertex(1, -1));

int main()
{
	while (1)
	{
		se(g.V)sc(g.E)

		g.clear();			// clear() 要放在读入 V 之后，否则无法正确 clear 
		g.inputEdges();

		g.src_index = 0;

		g.initDijkstra();
		g.Dijkstra();
		g.printMinDist();
	}
	return 0;
}
