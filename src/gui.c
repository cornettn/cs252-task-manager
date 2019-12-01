#include "gui.h"

#include <stdio.h>
#include <math.h>
//`#include <glg_cairo.h>
#include <cairo.h>

#include "logger.h"
#include "sys_info.h"
#include "pid_parser.h"
#include "graph_info.h"
#include "file_system.h"
//#include "slope/slope.h"

#define UNUSED(x) (void)(x)
#define BUF_SIZE (1024)
#define PROCESS_PAGE_NUM (1)
#define RIGHT_CLICK (3)

/* Function Declarations */

int static link_all_buttons();
void static link_menu_bar_buttons();
void static link_monitor_buttons();
void static link_edit_buttons();
void static link_view_buttons();
void static link_help_buttons();
void static init_system_tab();
void static init_process_view();
void static init_resource_graphs();
void static init_file_systems();
void static link_tabs();

/* Global Variables */

GtkBuilder *builder;
GtkCheckMenuItem *my_proc;
GtkCheckMenuItem *all_proc;
GtkCheckMenuItem *active_proc;

/*
 * ##############################################
 * START MENU BAR FUNCTIONS
 * ##############################################
 */

/*
 * This function is used to quit to application.
 */

void quit_app(GtkWidget *widget, gpointer data) {
  mylog("Quitting Application");
  stop_logging();
  g_object_unref(builder);
  gtk_main_quit();
  exit(1);
} /* quit_app() */

/*
 * This function is called whenever the user is adjusting what type of
 * view they want. i.e. All Processes, My Processes, Active Processes.
 * This function is reposnsible for changing the gui accordingly.
 */

void toggle_process_view(GtkWidget *widget, gpointer data) {
  GtkCheckMenuItem *toggle = data;

  /* Only change the other checks if toggle has been turned on */

  if (gtk_check_menu_item_get_active(toggle)) {

    /* Only turn the toggles off if they are on and are not the
     * intended toggle */

    if ((my_proc != toggle) &&
        (gtk_check_menu_item_get_active(my_proc)) ) {
      gtk_check_menu_item_set_active(my_proc, FALSE);
    }

    if ((all_proc != toggle) &&
        (gtk_check_menu_item_get_active(all_proc)) ) {
      gtk_check_menu_item_set_active(all_proc, FALSE);
    }

    if ((active_proc != toggle) &&
        (gtk_check_menu_item_get_active(active_proc)) ) {
      gtk_check_menu_item_set_active(active_proc, FALSE);
    }
  }
} /* toggle_process_view() */

/*
 * This function is used to initialize the task manager with the
 * appropiate information.
 * NOTE: This function links all of the buttons together.
 */

int init_task_manager(GtkBuilder *gui_builder) {
	mylog("Initializing Task Manager");
  builder = gui_builder;

  GObject *toggle = gtk_builder_get_object(builder, "view_my_processes");
  my_proc = (GtkCheckMenuItem *) toggle;

  toggle = gtk_builder_get_object(builder, "view_all_processes");
  all_proc = (GtkCheckMenuItem *) toggle;

  toggle = gtk_builder_get_object(builder, "view_active_processes");
  active_proc = (GtkCheckMenuItem *) toggle;

  link_all_buttons();
  return 1;
} /* init_task_manager() */

/*
 * This function is used to link all of the buttons in the task manager
 * together as well as link the buttons to their appropiate function.
 */

int static link_all_buttons() {
  mylog("Link All Buttons");
  mylog("Link Main Quit");
  GObject *main_window = gtk_builder_get_object(builder, "main_window");
  g_signal_connect(main_window, "destroy", G_CALLBACK(quit_app), NULL);

  link_menu_bar_buttons();
  link_tabs();
  init_system_tab();
  init_process_view();
  init_resource_graphs();
  init_file_systems();

  return 1;
} /* link_all_buttons() */

/*
 * This function is used to link all the buttons of the menu bar
 * to their respective action.
 */

void static link_menu_bar_buttons() {
  link_monitor_buttons();
	link_edit_buttons();
	link_view_buttons();
	link_help_buttons();
} /* link_menu_bar_buttons() */

/*
 * This function is used to link all of the 'Monitor' menu bar buttons.
 */

void static link_monitor_buttons() {
  GObject *quit = gtk_builder_get_object(builder, "monitor_quit_button");
	g_signal_connect(quit, "activate", G_CALLBACK(quit_app), NULL);
} /* link_monitor_buttons() */

/*
 * This function is used to link all of the 'Edit' menu bar buttons.
 */

void static link_edit_buttons() {
	return;
} /* link_edit_buttons() */

/*
 * This function is used to link of the 'View' menu bar buttons.
 */

void static link_view_buttons() {
  g_signal_connect(G_OBJECT(my_proc), "toggled", G_CALLBACK(toggle_process_view), my_proc);
  g_signal_connect(G_OBJECT(all_proc), "toggled", G_CALLBACK(toggle_process_view), all_proc);
  g_signal_connect(G_OBJECT(active_proc), "toggled", G_CALLBACK(toggle_process_view), active_proc);
} /* link_view_buttons() */

/*
 * This function is used to link the 'Help' menu bar buttons.
 */

void static link_help_buttons() {
	return;
} /* link_help_buttons() */


/*
 * ##############################################
 * END MENU BAR FUNCTIONS
 * ##############################################
 */

/*
 * ##############################################
 * START ALL TAB FUNCTIONS
 * ##############################################
 */

/*
 * This function is used grey out the process views depending on the page
 * selected.
 */

void process_view_visibility(GtkNotebook *widget, GtkWidget *page, guint page_num,
                             gpointer data) {

  /* Grey out the process views if not on process page */

  if ( page_num == PROCESS_PAGE_NUM ) {
    gtk_widget_set_sensitive(GTK_WIDGET(my_proc), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(all_proc), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(active_proc), TRUE);
    return;
  }

  gtk_widget_set_sensitive(GTK_WIDGET(my_proc), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(all_proc), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(active_proc), FALSE);
} /* process_view_visibiliyu() */

/*
 * This function is used to link all of tabs to specific functions.
 */

void static link_tabs() {
  mylog("Linking Tabs");

  /* Initialize the views to be greyed out */

  gtk_widget_set_sensitive(GTK_WIDGET(my_proc), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(all_proc), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(active_proc), FALSE);

  /* Link the page switches */

  GObject *all_tabs = gtk_builder_get_object(builder, "all_tabs");
  g_signal_connect(G_OBJECT(all_tabs), "switch-page",
                   G_CALLBACK(process_view_visibility), NULL);
} /* link_tabs() */


/*
 * ##############################################
 * END ALL TAB FUNCTIONS
 * ##############################################
 */

/*
 * ##############################################
 * START SYSTEM TAB FUNCTIONS
 * ##############################################
 */

void static init_system_tab() {

  /* Initialize sys_info functions */

  if (init_sys_info() < 0) {
    mylog("Error initializing sys_info");
  }

  /* Get system information using sys_info.c */

  sys_info info = {0};
  fill_info(&info);

  /* Retrieve labels from builder */

  GObject *os_type = gtk_builder_get_object(builder, "os_type_label");
  GObject *os_verison = gtk_builder_get_object(builder, "os_release_version_label");
  GObject *kernel_version = gtk_builder_get_object(builder, "kernel_version_label");
  GObject *mem_amount = gtk_builder_get_object(builder, "memory_label");
  GObject *proc_type = gtk_builder_get_object(builder, "processor_type_label");
  GObject *disk_space = gtk_builder_get_object(builder, "disk_space_label");

  /* Set labels to correct text */

  gtk_label_set_text(GTK_LABEL(os_type), info.OS_name);
  gtk_label_set_text(GTK_LABEL(os_verison), info.OS_version);
  gtk_label_set_text(GTK_LABEL(kernel_version), info.kernel_version);
  gtk_label_set_text(GTK_LABEL(mem_amount), info.mem_total);
  gtk_label_set_text(GTK_LABEL(proc_type), info.CPU_info);

  char disk_storage[1024] = {0};
  sprintf(disk_storage, "%.2f", info.disk_storage);
  gtk_label_set_text(GTK_LABEL(disk_space), disk_storage);

}

/*
 * ##############################################
 * END SYSTEM TAB FUNCTIONS
 * ##############################################
 */

/*
 * ##############################################
 * START PROCESSES TAB FUNCTIONS
 * ##############################################
 */



/*
 * This function will display the options for processes when a user right
 * clicks a process.
 */

void show_process_options(GtkTreeView *tree_view,
                          GtkTreePath *path,
                          GtkTreeViewColumn *column,
                          gpointer user_data) {
  GtkMenu *menu = GTK_MENU(gtk_builder_get_object(builder,
            "proc_options"));
  gtk_menu_popup_at_pointer(menu, NULL);
} /* show_process_options() */


void mouse_click(GtkWidget *widget,
                   GdkEvent *event,
                   gpointer user_data) {
  if (event->type == GDK_BUTTON_PRESS &&
      ((GdkEventButton *) event)->button == RIGHT_CLICK) {

    gint x = (gint) ((GdkEventButton *) event)->x;
    gint y = (gint) ((GdkEventButton *) event)->y;

    GtkTreePath **path = malloc(sizeof(char *));


    gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(widget),
                                  x, y, path, NULL, NULL, NULL);
    show_process_options(GTK_TREE_VIEW(widget),
                         *path, NULL, NULL);
  }
}

/*
 * This function is used to append a row to end of the list store.
 */

void static add_row_to_processes(GtkListStore *list_store,
                                 GtkTreeIter *iter,
                                 process_t *proc) {
  /* Enum for the different columns in the tree view */

  enum {NAME, STATUS, CPU, ID, MEM};
  gint cols[] = {NAME, STATUS, CPU, ID, MEM};

  /* Initialize GValues */

  GValue name = G_VALUE_INIT;
  GValue status = G_VALUE_INIT;
  GValue cpu = G_VALUE_INIT;
  GValue pid = G_VALUE_INIT;
  GValue mem = G_VALUE_INIT;

  /* Cast GValues to appropiate type */

  g_value_init(&name, G_TYPE_STRING);
  g_value_init(&status, G_TYPE_STRING);
  g_value_init(&cpu, G_TYPE_STRING);
  g_value_init(&pid, G_TYPE_STRING);
  g_value_init(&mem, G_TYPE_STRING);

  /* Set the values according to proc */

  g_value_set_static_string(&name, proc->proc_name);
  g_value_set_static_string(&status, proc->state);
  g_value_set_static_string(&cpu, proc->cpu_time);
  g_value_set_static_string(&pid, proc->pid);
  g_value_set_static_string(&mem, proc->memory);

  GValue vals[] = {name, status, cpu, pid, mem};

  /* Append the row with the values to the list store */

  gtk_list_store_append(list_store, iter);
  gtk_list_store_set_valuesv(list_store, iter, cols, vals, 5);

} /* add_row_to_processes() */

/*
 * This function is used to display the processes.
 */

void display_procs(process_t **procs) {

  /* Get references for tree view */

  GObject *tree_view = gtk_builder_get_object(builder, "processes_tree_view");
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree_view));

  /* Get the tree path of the first row */

  GtkTreePath *tree_path = gtk_tree_path_new_first();

  /* Create Tree iterator */

  GtkTreeIter iter = {0};
  gtk_tree_model_get_iter(model, &iter, tree_path);

  GtkListStore *list_store =
    GTK_LIST_STORE(gtk_builder_get_object(builder, "processes_list_store"));

  /* Add processes to tree view */

  int counter = 0;
  process_t *curr_proc = procs[counter];
  while (curr_proc != NULL) {
    add_row_to_processes(list_store, &iter, curr_proc);
    curr_proc = procs[++counter];
  }
} /* display_processes() */


//PLACEHOLDER
process_t **get_all_process() {
  /* This function is a placeholder */

  process_t *proc = (process_t *) malloc(sizeof(process_t));
  proc->pid = "pid";
  proc->proc_name = "name";
  proc->state = "state";
  proc->memory = "mem";
  proc->cpu_time = "cpu_time";

  process_t **procs = (process_t **) malloc(2 * sizeof(process_t *));
  procs[0] = proc;
  procs[1] = NULL;
  return procs;
}

/*
 * This function is used to initialize the process tab.
 */

void static init_process_view() {

  /* Initialize view to see all processes */

  gtk_check_menu_item_set_active(all_proc, TRUE);

  // TODO Get all of the processes from the pid_parser

  process_t **procs = get_all_process();
  display_procs(procs);


  GObject *tree_view = gtk_builder_get_object(builder, "processes_tree_view");
  g_signal_connect(G_OBJECT(tree_view), "button-press-event",
      G_CALLBACK(mouse_click), NULL);
  g_signal_connect(G_OBJECT(tree_view), "row-activated",
      G_CALLBACK(show_process_options), NULL);

} /* init_process_view() */

/*
 * ##############################################
 * END PROCESSES TAB FUNCTIONS
 * ##############################################
 */

/*
 * ##############################################
 * START RESOURCE TAB FUNCTIONS
 * ##############################################
 */


#define ZOOM_X (100.0)
#define ZOOM_Y (100.0)

gfloat f (gfloat x) {
  return 0.03 * pow(x, 3);
}

/*
 * This function is used to draw the cpu history graph. This
 * function is called whenever the "draw" action is emitted by the drawing
 * area.
 */

gboolean static draw_cpu(GtkWidget *widget, cairo_t *cr,
                             gpointer user_data) {
  /* GtkDrawingArea size */

  GdkRectangle da;

  /* Pixels between each point */

  gdouble dx = 5.0, dy = 5.0;
  gdouble i, clip_x1 = 0.0, clip_y1 = 0.0, clip_x2 = 0.0, clip_y2 = 0.0;
  GdkWindow *window = gtk_widget_get_window(widget);

  /* Determine GtkDrawingArea dimensions */

  gdk_window_get_geometry (window,
            &da.x,
            &da.y,
            &da.width,
            &da.height);

  /* Draw on a white background */

  cairo_set_source_rgb (cr, 255,255, 255);
  cairo_paint (cr);

  /* Change the transformation matrix */

  cairo_translate (cr, da.width / 2, da.height / 2);
  cairo_scale (cr, ZOOM_X, -ZOOM_Y);

  /* Determine the data points to calculate (ie. those in the clipping zone */

  cairo_device_to_user_distance (cr, &dx, &dy);
  cairo_clip_extents (cr, &clip_x1, &clip_y1, &clip_x2, &clip_y2);
  cairo_set_line_width (cr, dx);

  /* Draws x and y axis */

  cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
  cairo_move_to (cr, clip_x1, clip_y1);
  cairo_line_to (cr, clip_x2, clip_y1);
  cairo_move_to (cr, clip_x1, clip_y1);
  cairo_line_to (cr, clip_x1, clip_y2);
  cairo_stroke (cr);

  /* Get data points */

  /* Link each data point */

  for (i = clip_x1; i < clip_x2; i += dx)
      cairo_line_to (cr, i, f (i));


  /* Draw the curve */

  cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
  cairo_stroke (cr);

  return FALSE;
} /* draw_cpu() */

/*
 * This function is used to draw the memory and swap history graph. This
 * function is called whenever the "draw" action is emitted by the drawing
 * area.
 */

gboolean static draw_memory_swap(GtkWidget *widget, cairo_t *cr,
                             gpointer user_data) {

  GdkRectangle da;            /* GtkDrawingArea size */
  gdouble dx = 5.0, dy = 5.0; /* Pixels between each point */
  gdouble i, clip_x1 = 0.0, clip_y1 = 0.0, clip_x2 = 0.0, clip_y2 = 0.0;
  GdkWindow *window = gtk_widget_get_window(widget);

  /* Determine GtkDrawingArea dimensions */

  gdk_window_get_geometry (window,
            &da.x,
            &da.y,
            &da.width,
            &da.height);

  /* Draw on a white background */

  cairo_set_source_rgb (cr, 255,255, 255);
  cairo_paint (cr);

  /* Change the transformation matrix */

  cairo_translate (cr, da.width / 2, da.height / 2);
  cairo_scale (cr, ZOOM_X, -ZOOM_Y);

  /* Determine the data points to calculate (ie. those in the clipping zone */

  cairo_device_to_user_distance (cr, &dx, &dy);
  cairo_clip_extents (cr, &clip_x1, &clip_y1, &clip_x2, &clip_y2);
  cairo_set_line_width (cr, dx);

  /* Draws x and y axis */

  cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
  cairo_move_to (cr, clip_x1, clip_y1);
  cairo_line_to (cr, clip_x2, clip_y1);
  cairo_move_to (cr, clip_x1, clip_y1);
  cairo_line_to (cr, clip_x1, clip_y2);
  cairo_stroke (cr);

  /* Get data points */
//  ms_hist *data = get_memswap();

  /* Link each data point */

  for (i = clip_x1; i < clip_x2; i += dx)
      cairo_line_to (cr, i, f (i));

//  UNUSED(data);

  /* Draw the curve */

  cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
  cairo_stroke (cr);

  return FALSE;
} /* draw_memory_swap() */

/*
 * This function is sued to draw the network history graph. This function
 * is called whenever the "draw" action is emitted by the drawing area.
 */

gboolean static draw_net(GtkWidget *widget, cairo_t *cr,
                             gpointer user_data) {

  GdkRectangle da;            /* GtkDrawingArea size */
  gdouble dx = 5.0, dy = 5.0; /* Pixels between each point */
  gdouble i, clip_x1 = 0.0, clip_y1 = 0.0, clip_x2 = 0.0, clip_y2 = 0.0;
  GdkWindow *window = gtk_widget_get_window(widget);

  /* Determine GtkDrawingArea dimensions */

  gdk_window_get_geometry (window,
            &da.x,
            &da.y,
            &da.width,
            &da.height);

  /* Draw on a white background */

  cairo_set_source_rgb (cr, 255,255, 255);
  cairo_paint (cr);

  /* Change the transformation matrix */

  cairo_translate (cr, da.width / 2, da.height / 2);
  cairo_scale (cr, ZOOM_X, -ZOOM_Y);

  /* Determine the data points to calculate (ie. those in the clipping zone */

  cairo_device_to_user_distance (cr, &dx, &dy);
  cairo_clip_extents (cr, &clip_x1, &clip_y1, &clip_x2, &clip_y2);
  cairo_set_line_width (cr, dx);

  /* Draws x and y axis */

  cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
  cairo_move_to (cr, clip_x1, clip_y1);
  cairo_line_to (cr, clip_x2, clip_y1);
  cairo_move_to (cr, clip_x1, clip_y1);
  cairo_line_to (cr, clip_x1, clip_y2);
  cairo_stroke (cr);

  /* Get data points */
//  ms_hist *data = get_memswap();

  /* Link each data point */

  for (i = clip_x1; i < clip_x2; i += dx)
      cairo_line_to (cr, i, f (i));

//  UNUSED(data);

  /* Draw the curve */

  cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
  cairo_stroke (cr);

  return FALSE;
} /* draw_net() */

/*
 * This function is used to initialize the resource graphs.
 */

void static init_resource_graphs() {
  GObject *mem_swap_graph = gtk_builder_get_object(builder, "memory_and_swap_drawing_area");
  GObject *cpu_graph = gtk_builder_get_object(builder, "cpu_usage_drawing_area");
  GObject *net_graph = gtk_builder_get_object(builder, "network_history_drawing_area");

  g_signal_connect(G_OBJECT(mem_swap_graph), "draw", G_CALLBACK(draw_memory_swap), NULL);
  g_signal_connect(G_OBJECT(cpu_graph), "draw", G_CALLBACK(draw_cpu), NULL);
  g_signal_connect(G_OBJECT(net_graph), "draw", G_CALLBACK(draw_net), NULL);
} /* init_resource_graphs() */


/*
 * ##############################################
 * END RESOURCE TAB FUNCTIONS
 * ##############################################
 */


/*
 * ##############################################
 * START FILE SYSTEMS TAB FUNCTIONS
 * ##############################################
 */

/*
 * This function is used to append a mount to the file system list store.
 */

void static add_row_to_file_system(GtkListStore *list_store,
                                   GtkTreeIter *iter,
                                   mount *curr_mount) {

  /* Create an enum for the different column numbers in the tree view */

  enum {DEVICE, DIRECTORY, TYPE, TOT, FREE, AVAIL, UNUSE};
  gint cols[] = {DEVICE, DIRECTORY, TYPE, TOT, FREE, AVAIL, UNUSE};

  /* Initialize GValues */

  GValue dev = G_VALUE_INIT;
  GValue dir = G_VALUE_INIT;
  GValue type = G_VALUE_INIT;
  GValue tot = G_VALUE_INIT;
  GValue free = G_VALUE_INIT;
  GValue avail = G_VALUE_INIT;
  GValue unuse = G_VALUE_INIT;

  /* Cast GValues to approiate type */

  g_value_init(&dev, G_TYPE_STRING);
  g_value_init(&dir, G_TYPE_STRING);
  g_value_init(&type, G_TYPE_STRING);
  g_value_init(&tot, G_TYPE_DOUBLE);
  g_value_init(&free, G_TYPE_DOUBLE);
  g_value_init(&avail, G_TYPE_DOUBLE);
  g_value_init(&unuse, G_TYPE_DOUBLE);

  /* Set the values according to the contents of curr_mount */

  g_value_set_static_string(&dev, curr_mount->dev_name);
  g_value_set_static_string(&dir, curr_mount->dev_dir);
  g_value_set_static_string(&type, curr_mount->dev_type);
  g_value_set_double(&tot, curr_mount->dev_total_space);
  g_value_set_double(&free, curr_mount->dev_free_space);
  g_value_set_double(&avail, curr_mount->dev_avail_space);
  g_value_set_double(&unuse, curr_mount->dev_used_space);

  GValue vals[] = {dev, dir, type, tot, free, avail, unuse};

  /* Append the row with the values to the list store */

  gtk_list_store_append(list_store, iter);
  gtk_list_store_set_valuesv(list_store, iter, cols, vals, 7);

} /* add_row_to_file_system() */

/*
 * This function initializes the file_systems tab to display information.
 */

void static init_file_systems() {
  mount **mounts = get_mount_list();
  if (mounts == NULL) {
    mylog("Error Getting mount list");
    return;
  }

  /* Get references for Tree View */

  GObject *file_system_tree_view =
    gtk_builder_get_object(builder, "file_system_tree");

  GtkTreeModel *model =
    gtk_tree_view_get_model(GTK_TREE_VIEW(file_system_tree_view));

  /* Get the tree path of the first row */

  GtkTreePath *tree_path = gtk_tree_path_new_first();

  /* Get the tree iterator and tree store to edit rows */

  GtkTreeIter iter = {0};
  gtk_tree_model_get_iter(model, &iter, tree_path);

  GtkListStore *list_store =
    GTK_LIST_STORE(gtk_builder_get_object(builder, "file_system_list_store"));


  /* Add all of the mounts to the list store */

  //TODO Do not limit the number of mounts to 20
  int counter = 0;
  mount *curr_mount = mounts[counter];
  while(curr_mount != NULL) {
    add_row_to_file_system(list_store, &iter, curr_mount);
    curr_mount = mounts[++counter];
    if (counter > 20) {
      break;
    }
  }
} /* init_file_systems() */

/*
 * ##############################################
 * END FILE SYSTEMS TAB FUNCTIONS
 * ##############################################
 */
