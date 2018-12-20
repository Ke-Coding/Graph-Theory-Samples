#include <cstdio>
#include <cstring>


typedef int DistType;
typedef int DistSrcType;

#define sc(x) {register char _c=getchar(),_v=1;for(x=0;_c<48||_c>57;_c=getchar())if(_c==45)_v=-1;for(;_c>=48&&_c<=57;x=(x<<1)+(x<<3)+_c-48,_c=getchar());x*=_v;}
#define se(x) {register char _c=getchar(),_v=1;for(x=0;_c<48||_c>57;_c=getchar())if(_c==45)_v=-1;else if(_c==-1)return 0;for(;_c>=48&&_c<=57;x=(x<<1)+(x<<3)+_c-48,_c=getchar());x*=_v;}
void PRT(const DistSrcType a){if(a>=10)PRT(a/10);putchar(a%10+48);}


// 【注意】 用 long long 时，把 INF 改成 0x3f3f3f3f3f3f3f3fLL，int 时可以用 0x3f3f3f3f 
constexpr DistSrcType INF(0x3f3f3f3f3f3f3f3fLL);
constexpr int MV(100003);
constexpr int ME(1000003);


class SGraph
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
		bool vis[MV];
		DistSrcType dist_src[MV];
		int queue[ME];

	public:

		SGraph(void) { }

		inline void clear(void)
		{
			memset(head, 0, sizeof(*head) * (V+1));	// 【链式前向星的清空】和顶点从0从1编号无关 
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

		void SPFA(void)
		{
			memset(dist_src, 0x3f, sizeof(*dist_src) * (V+1));
			memset(vis, false, sizeof(*vis) * (V+1));

			int u, v;
			DistType dist;
			int h = 0, t = 0;

			vis[src_index] = true;
			dist_src[src_index] = 0;
			queue[t++] = src_index;

			while (h != t)
			{
				u = queue[h++];
				vis[u] = false;
				for (int i=head[u]; i; i=edge[i].next)			// 【链式前向星的遍历】和顶点从0从1编号无关 
				{
					v = edge[i].dest, dist = edge[i].dist;
					if (dist_src[v] > dist_src[u] + dist)
					{
						dist_src[v] = dist_src[u] + dist;
						if (!vis[v])
						{
							vis[v] = true;
							queue[t++] = v;
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

		g.SPFA();
		g.printMinDist();
	}
	return 0;
}
