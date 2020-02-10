/* Home-made implementation of the non-GPU graphic library, based on GTK.
Developed by Pavlenco Igor Alexander just for fun.
Distributed under GNU General Public License v3.0.
Version 0.10.
*/

#ifndef CUBE_GTK_H
#define CUBE_GTK_H
#include "Cube_GTK.h"

#include <inttypes.h>
#include <gtk/gtk.h>
#include <math.h>

#define PI 3.14159265
#define CUBE_LENGTH 150

//default resolution and offset
#define DEFAULT_Y 1400
#define DEFAULT_Z 900
#define DEFAULT_X 0

#define DEFAULT_Y_OFFSET DEFAULT_Y/2
#define DEFAULT_Z_OFFSET DEFAULT_Z/2
#define DEFAULT_X_OFFSET DEFAULT_X/2

#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2

#define CUBE 0

typedef struct
{
    double x;
    double y;
    double z;
} type_point;

typedef struct primitive
{
    type_point* p_figure;
    type_point* p_original;
    uint16_t len_p_figure;
    struct primitive* next_primitive;
    struct primitive* previous_primitive;
    uint8_t type_of_primitive;

} type_primitive;

typedef struct scene
{
    uint16_t angle_x;
    uint16_t angle_y;
    uint16_t angle_z;
    type_point axis_center;
    type_primitive* p_primitive;
    struct scene* next_scene;
    struct scene* previous_scene;
    uint8_t current_primitive;
    uint8_t work_primitive;

} type_scene;

typedef struct
{
    type_scene* scene;
    uint8_t current_scene;
    uint8_t work_scene;
} type_space;

//"Private functions"
inline static void delete_primitive(type_primitive* primitive, uint8_t number);
inline static void delete_scene(type_scene* scene, uint8_t number);
inline static void free_primitives(type_primitive* primitive);
inline static void free_scenes(type_scene* scene);
inline static uint8_t initialize_next_primitive(type_primitive* primitive, type_point point, uint8_t type_figure);
inline static uint8_t initialize_next_scene(type_scene* scene, type_point point);
inline static uint8_t initialize_scene(type_point point);
inline static void initialize_cube(type_primitive* primitive, type_point point);
inline static void initialize_figure(type_primitive* primitive, type_point point, uint8_t type);
inline static type_primitive* get_current_primitive();
inline static type_primitive* get_primitive_number(type_primitive* primitive, uint8_t num);
inline static type_scene* get_current_scene();
inline static type_scene* get_scene_number(type_scene* scene, uint8_t num);
inline static uint8_t alt_angle_scene_service(uint8_t num, uint16_t angle, uint8_t axis);
inline static void draw_primitives(GtkWidget *widget, type_primitive *primitive, uint8_t num);
inline static void draw_scenes(GtkWidget *widget, type_scene *scene, uint8_t num);
inline static void draw_cube(GtkWidget *widget, type_primitive *primitive, uint8_t num);
inline static void draw_linear_interpolation(GtkWidget *widget, type_point* point_a, type_point* point_b, uint8_t num);

//"Public funcions"
void delete_current_primitive();
void delete_current_scene();
void free_all();
uint8_t create_primitive(uint8_t type_figure, type_point point);
void create_scene(type_point point);
void alt_angle_scene(uint16_t angle, uint8_t axis);
void alt_all_scenes(uint16_t angle, uint8_t axis);
void draw_all(GtkWidget* widget);
void mod_current_primitive(int8_t n);
uint8_t has_current_primitive();
void mod_current_scene(int8_t n);
uint8_t has_current_scene();

void draw_brush(GtkWidget* widget, gdouble x, gdouble y);

#endif
