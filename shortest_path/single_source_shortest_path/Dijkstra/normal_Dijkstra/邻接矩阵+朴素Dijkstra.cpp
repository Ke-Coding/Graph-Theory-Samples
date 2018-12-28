#include <cstdio>
#include <cstring>


typedef int DistType;
typedef int DistSrcType;

#define sc(x) {register char _c=getchar(),_v=1;for(x=0;_c<48||_c>57;_c=getchar())if(_c==45)_v=-1;for(;_c>=48&&_c<=57;x=(x<<1)+(x<<3)+_c-48,_c=getchar());x*=_v;}
#define se(x) {register char _c=getchar(),_v=1;for(x=0;_c<48||_c>57;_c=getchar())if(_c==45)_v=-1;else if(_c==-1)return 0;for(;_c>=48&&_c<=57;x=(x<<1)+(x<<3)+_c-48,_c=getchar());x*=_v;}
template<typename T>void PRT(const T a){if(a<0){PC(45),PRT(-a);return;}if(a>=10)PRT(a/10);PC(a%10+48);}


// 【注意】 用 long long 时，把 INF 改成 0x3f3f3f3f3f3f3f3fLL，int 时可以用 0x3f3f3f3f 
constexpr DistSrcType INF(0x3f3f3f3f3f3f3f3fLL);
constexpr DistType NOT_CONNECTED(0x3f3f3f3f3f3f3f3fLL);
constexpr int MV(1003);
constexpr int ME(1000003);


class MGraph		// 邻接矩阵存储的图
{
	public:

		DistType g[MV][MV];

		int V, E;
		int src_index;
		bool collected[MV];			// 记录Dijkstra中的收录情况
		DistSrcType dist_src[MV];	// 到源点的路径
//		int path[MV];

	public:

		MGraph(void) { }

		inline void clear(void)
		{
			memset(g, 0x3f, sizeof(g));
		}

		void inputEdges(void)
		{
			int src, dest;
			DistType dist;
			for (int e=0; e<E; ++e)
			{
				sc(src)sc(dest)sc(dist)
				g[src][dest] = g[dest][src] = dist;
			}
		}

		void initDijkstra()
		{
			memset(collected, false, sizeof(*collected) * (V+1));		// 点都没有被收录
			memset(dist_src, 0x3f, sizeof(*dist_src) * (V+1));			// 到源点的距离都是无穷
			for (int v=0; v<V; ++v)										// 从0开始，循环就是0 -> V-1
				g[v][v] = 0;											// 初始化对角元 
//			memset(path, -1, sizeof(*path) * (V+1));					// 初始化路径

			collected[src_index] = true, dist_src[src_index] = 0;		// 收录源点，更新源点到源点的距离(是0)

			for (int v=0; v<V; ++v)										// 从0开始，循环就是0 -> V-1
			{
				if (v != src_index)
				{
					dist_src[v] = g[src_index][v];						// 更新源点直接邻接点的距离
//					path[v] = src_index;								// 初始化第一批路径
				}
			}
		}

		void Dijkstra()
		{
			int u, v;
			DistType dist;
			while (1)
			{
				DistSrcType min_d = INF;
				for (int i=0; i<V; ++i)					// 从0开始，循环就是0 -> V-1
				{
					if (!collected[i] && dist_src[i] < min_d)
					{
						min_d = dist_src[i];
						u = i;
					}
				}
				if (min_d == INF)
					break;								// 没有未收录顶点了

				if (!collected[u])						// 如果没有收录
				{
					collected[u] = true;				// 收录

					for (int v=0; v<V; ++v)				// 顶点从 0 开始编号，循环就是 0 -> V-1
					{
						// 对u的所有未访问邻接点进行松弛操作
						if (!collected[v] && g[u][v] != NOT_CONNECTED)	// 本来还要打一个 u!=v，但是前面访问了u，所以v==u时 if中第一个条件就不满足 
						{
							if (dist_src[v] > dist_src[u] + g[u][v])
							{
								dist_src[v] = dist_src[u] + g[u][v];
//								path[v] = u;							// 松弛的时候记录路径
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

MGraph g;

int main()
{
	while (1)
	{
		se(g.V)sc(g.E)

		g.clear();			// clear() 要放在读入 V 之后，否则无法正确mem0
		g.inputEdges();

		g.src_index = 0;

		g.initDijkstra();
		g.Dijkstra();
		g.printMinDist();
	}
	return 0;
}
