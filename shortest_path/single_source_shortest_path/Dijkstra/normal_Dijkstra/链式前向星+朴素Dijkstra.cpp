#include <cstdio>
#include <cstring>


typedef int DistType;
typedef int DistSrcType;

#define sc(x) {register char _c=getchar(),_v=1;for(x=0;_c<48||_c>57;_c=getchar())if(_c==45)_v=-1;for(;_c>=48&&_c<=57;x=(x<<1)+(x<<3)+_c-48,_c=getchar());x*=_v;}
#define se(x) {register char _c=getchar(),_v=1;for(x=0;_c<48||_c>57;_c=getchar())if(_c==45)_v=-1;else if(_c==-1)return 0;for(;_c>=48&&_c<=57;x=(x<<1)+(x<<3)+_c-48,_c=getchar());x*=_v;}
template<typename T>void PRT(const T a){if(a<0){PC(45),PRT(-a);return;}if(a>=10)PRT(a/10);PC(a%10+48);}


// 【注意】 用 long long 时，把 INF 改成 0x3f3f3f3f3f3f3f3fLL，int 时可以用 0x3f3f3f3f 
constexpr DistSrcType INF(0x3f3f3f3f3f3f3f3fLL);
constexpr int MV(100003);
constexpr int ME(1000003);


class SGraph		// 链式前向星存储的图
{
	public:

		struct Edge
		{
			DistType dist;
			int dest;
			int next;
		} edge[ME];
		int head[MV];

		int V, E;
		int src_index;
		bool collected[MV];		// 记录Dijkstra中的收录情况
		DistSrcType dist_src[MV];	// 到源点的路径
//		int path[MV];

	public:

		SGraph(void) { }

		inline void clear(void)
		{
			memset(head, 0, sizeof(*head) * (V+1));			// 【链式前向星的清空】和顶点从0从1编号无关
		}

		void inputEdges(void)
		{
			int src, dest;
			DistType dist;
			
			/* 【链式前向星的构建】和顶点从0从1编号无关
			 *    且读边循环必须从1开始，或者用tot读的时候也一定要让第一条边的编号是1
			 *    因为认为下标0是非法的（遍历邻接边的时候i是用head[u]初始化的，条件是i，所以head[u]千万不能是0） 
			 *    所以head的每一个有效元素都是非0的 
			 *    而head[u]放的是u顶点的第一条边的编号，所以边的编号必须从1开始 
			 */
			for (int e=1, EE=E*2; e<=EE; ++e)
			{
				sc(src)sc(dest)sc(dist)
				edge[e].next = head[src];
				edge[e].dest = dest;
				edge[e].dist = dist;
				head[src] = e;

				++e;

				edge[e].next = head[dest];
				edge[e].dest = src;
				edge[e].dist = dist;
				head[dest] = e;
			}
		}

		void initDijkstra()
		{
			memset(collected, false, sizeof(*collected) * (V+1));		// 点都没有被收录
			memset(dist_src, 0x3f, sizeof(*dist_src) * (V+1));		// 到源点的距离都是无穷
//			memset(path, -1, sizeof(*path) * (V+1));			// 初始化路径

			collected[src_index] = true, dist_src[src_index] = 0;		// 收录源点，更新源点到源点的距离(是0)

			for (int i=head[src_index]; i; i=edge[i].next)			// 【链式前向星的遍历】for的条件和顶点从0从1编号无关
			{
				dist_src[edge[i].dest] = edge[i].dist;			// 更新源点直接邻接点的距离
//				path[edge[i].dest] = src_index;				// 初始化第一批路径
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

					for (int i=head[u]; i; i=edge[i].next)	// 【链式前向星的遍历】for的条件和顶点从0从1编号无关
					{
						// 对这个顶点的所有邻接点进行松弛操作
						v = edge[i].dest, dist = edge[i].dist;
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

SGraph g;

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
