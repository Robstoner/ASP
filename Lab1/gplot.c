#include "gplot.h"

gplot_t *gplot_init()
{
  gplot_t *g = (gplot_t *)malloc(sizeof(gplot_t));
  g->pipe = popen("gnuplot -persist", "w");
  g->cmd = (char *)malloc(GPLOT_MAX_CMD);
  g->plot_count = 0;
  return g;
}

void gplot_free(gplot_t *g)
{
  pclose(g->pipe);
  fflush(g->pipe);
  free(g->cmd);
  free(g);
}

void gplot_cmd(gplot_t *g, char *cmd)
{
  strcpy(g->cmd, cmd);
  fprintf(g->pipe, "%s\n", g->cmd);
}

void gplot_style(gplot_t *g, char *style)
{
  fprintf(g->pipe, "set style %s\n", style);
}

void gplot_title(gplot_t *g, char *title)
{
  fprintf(g->pipe, "set title '%s'\n", title);
}

void gplot_xlabel(gplot_t *g, char *xlabel)
{
  fprintf(g->pipe, "set xlabel '%s'\n", xlabel);
}

void gplot_ylabel(gplot_t *g, char *ylabel)
{
  fprintf(g->pipe, "set ylabel '%s'\n", ylabel);
}

void gplot_plot(gplot_t *g, double *x, double *y, int n, char *style)
{
  g->plot_cmd[g->plot_count] = (char *)malloc(GPLOT_MAX_CMD);
  sprintf(g->plot_cmd[g->plot_count], "plot '-' with %s", style);
  gplot_cmd(g, g->plot_cmd[g->plot_count]);
  g->plot_count++;

  for (int i = 0; i < n; i++)
  {
    fprintf(g->pipe, "%f %f\n", x[i], y[i]);
  }
  fprintf(g->pipe, "e\n");
}

void gplot_plot_dat(gplot_t *g, char *filename, char *style) 
{
  g->plot_cmd[g->plot_count] = (char *)malloc(GPLOT_MAX_CMD);
  sprintf(g->plot_cmd[g->plot_count], "plot '%s' using %s", filename, style);
  gplot_cmd(g, g->plot_cmd[g->plot_count]);
  g->plot_count++;

  
}
