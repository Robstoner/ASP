#include <stdio.h>
#include <math.h>

#include "gplot.h"

double *func(double *x, int nr_items)
{
  double *v = (double *)malloc(nr_items * sizeof(double));
  for (int i = 0; i < nr_items; i++)
  {
    v[i] = pow(x[i], 2) + 2;
  }

  return v;
}

int main(int argc, char const *argv[])
{
  if (argc != 2) {
    printf("Usage: %s output_file_name\n", argv[0]);
    return 1;
  }

  int nr_items = 100;
  double *x = (double *)malloc(nr_items * sizeof(double));
  for (int i = 0; i < nr_items; i++)
  {
    x[i] = i;
  }

  double *y = func(x, nr_items);

  gplot_t *g = gplot_init();

  gplot_cmd(g, "set terminal png");
  char cmd[100];
  sprintf(cmd, "set output '%s'", argv[1]);
  gplot_cmd(g, cmd);

  gplot_title(g, "My function");
  gplot_xlabel(g, "x");
  gplot_ylabel(g, "y");
  gplot_style(g, "line 1 lt 1 lw 2 pt 6 ps 0.7 lc rgb 'blue'");

  gplot_plot(g, x, y, nr_items, "linespoints ls 1");

  gplot_free(g);
  free(x);
  free(y);

  return 0;
}