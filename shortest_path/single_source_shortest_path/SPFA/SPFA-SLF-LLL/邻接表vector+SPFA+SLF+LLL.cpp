#include <cstdio>
#include <cstring>
#include <vector>


typedef int DistType;
typedef int DistSrcType;

#define sc(x) {register char _c=getchar(),_v=1;for(x=0;_c<48||_c>57;_c=getchar())if(_c==45)_v=-1;for(;_c>=48&&_c<=57;x=(x<<1)+(x<<3)+_c-48,_c=getchar());x*=_v;}
#define se(x) {register char _c=getchar(),_v=1;for(x=0;_c<48||_c>57;_c=getchar())if(_c==45)_v=-1;else if(_c==-1)return 0;for(;_c>=48&&_c<=57;x=(x<<1)+(x<<3)+_c-48,_c=getchar());x*=_v;}
#define PC putchar
template<typename T>void PRT(const T a){if(a<0){PC(45),PRT(-a);return;}if(a>=10)PRT(a/10);PC(a%10+48);}


// 【注意】 用 long long 时，把 INF 改成 0x3f3f3f3f3f3f3f3fLL，int 时可以用 0x3f3f3f3f
constexpr DistSrcType INF(0x3f3f3f3f3f3f3f3fLL);
constexpr int MV(100003);
constexpr int ME(1000003);

class AGraph
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
		bool vis[MV];
		DistSrcType dist_src[MV];
		int _queue[2*ME], *queue = _queue + ME;

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

		void SPFA(void)
		{
			memset(dist_src, 0x3f, sizeof(*dist_src) * (V+1));
			memset(vis, false, sizeof(*vis) * (V+1));

			int u, v;
			DistType dist;
			int h = 0, t = 0, q_size = 0;
			long long sum = 0;

			vis[src_index] = true;
			dist_src[src_index] = 0;
			queue[t++] = src_index;

			while (h != t)
			{
				while (dist_src[queue[h]] * q_size > sum)
					queue[t++] = queue[h], ++h;

				u = queue[h++];
				sum -= dist_src[u], --q_size;
				vis[u] = false;
				for (auto &ed : edge[u])
				{
					// 对这个顶点的所有邻接点进行松弛操作
					v = ed.dest, dist = ed.dist;
					if (dist_src[v] > dist_src[u] + dist)
					{
						dist_src[v] = dist_src[u] + dist;
						if (!vis[v])
						{
							vis[v] = true;
							if (h < t && dist_src[queue[h]] > dist_src[v])
								queue[--h] = v;
							else
								queue[t++] = v;
							sum += dist_src[v], ++q_size;
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

		g.clear();			// clear() 要放在读入 V 之后，否则无法正确mem0
		g.inputEdges();

		g.src_index = 0;

		g.SPFA();
		g.printMinDist();
	}
	return 0;
}
