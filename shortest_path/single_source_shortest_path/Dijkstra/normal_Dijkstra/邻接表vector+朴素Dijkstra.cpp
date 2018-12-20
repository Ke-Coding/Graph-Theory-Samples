#include <cstdio>
#include <cstring>
#include <vector>


typedef int DistType;
typedef int DistSrcType;

#define sc(x) {register char _c=getchar(),_v=1;for(x=0;_c<48||_c>57;_c=getchar())if(_c==45)_v=-1;for(;_c>=48&&_c<=57;x=(x<<1)+(x<<3)+_c-48,_c=getchar());x*=_v;}
#define se(x) {register char _c=getchar(),_v=1;for(x=0;_c<48||_c>57;_c=getchar())if(_c==45)_v=-1;else if(_c==-1)return 0;for(;_c>=48&&_c<=57;x=(x<<1)+(x<<3)+_c-48,_c=getchar());x*=_v;}
void PRT(const DistSrcType a){if(a>=10)PRT(a/10);putchar(a%10+48);}


// 【注意】 用 long long 时，把 INF 改成 0x3f3f3f3f3f3f3f3fLL，int 时可以用 0x3f3f3f3f 
constexpr DistSrcType INF(0x3f3f3f3f3f3f3f3fLL);
constexpr int MV(100003);
constexpr int ME(1000003);


class AGraph		// 链式前向星存储的图
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

	public:

		AGraph(void) { }

		void clear(void)
		{
			for (int i=0; i<V; ++i)		// 源点编号从0开始，边集数组也是 
				edge[i].clear();
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
//				path[ed.dest] = src_index;
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
					break;						// 没有未收录顶点了

				if (!collected[u])					// 如果没有收录
				{
					collected[u] = true;				// 收录

					for (auto &ed : edge[u])
					{
						// 对这个顶点的所有邻接点进行松弛操作
						v = ed.dest, dist = ed.dist;
						if (!collected[v])
						{
							if (dist_src[v] > dist_src[u] + dist)
							{
								dist_src[v] = dist_src[u] + dist;
//								path[v] = u;		// 松弛的时候记录路径
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

AGraph g;

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
