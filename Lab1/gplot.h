#ifndef GPLOT_H
#define GPLOT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GPLOT_MAX_PLOT 10
#define GPLOT_MAX_CMD 100

typedef struct gplot_s
{
  FILE *pipe;
  char *cmd;
  int plot_count;
  char *plot_cmd[GPLOT_MAX_PLOT];
} gplot_t;

gplot_t *gplot_init();

void gplot_free(gplot_t *g);

void gplot_cmd(gplot_t *g, char *cmd);

void gplot_style(gplot_t *g, char *style);

void gplot_title(gplot_t *g, char *title);

void gplot_xlabel(gplot_t *g, char *xlabel);

void gplot_ylabel(gplot_t *g, char *ylabel);

void gplot_plot(gplot_t *g, double *x, double *y, int n, char *style);

#endif // GPLOT_H