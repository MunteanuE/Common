#include "Cube_GTK.h"
#ifdef CUBE_GTK_H

static type_space space = {0, 0, 0};
static uint8_t init = 0;

inline static void delete_primitive(type_primitive* primitive, uint8_t number)
{
    if( 0 == number )
    {
        free(primitive->p_figure);
        if( 0 != primitive->previous_primitive ) primitive->previous_primitive->next_primitive = primitive->next_primitive;
        if( 0 != primitive->next_primitive ) primitive->next_primitive->previous_primitive = primitive->previous_primitive;
        free(primitive);
        return;
    }
    number--;
    delete_primitive(primitive->next_primitive, number);
    return;
}

inline static void delete_scene(type_scene* scene, uint8_t number)
{
    if( 0 == number )
    {
        free_primitives(scene->p_primitive);
        if( 0 != scene->previous_scene ) scene->previous_scene->next_scene = scene->next_scene;
        if( 0 != scene->next_scene) scene->next_scene->previous_scene = scene->previous_scene;
        free(scene);
        return;
    }
    number--;
    delete_scene(scene->next_scene, number);
    return;
}

inline static void free_primitives(type_primitive* primitive)
{
    if( 0 == primitive->previous_primitive && 0 == primitive->next_primitive )
    {
        free(primitive);
        return;
    }
    if( 0 == primitive->next_primitive )
    {
        type_primitive* previous = primitive->previous_primitive;
        free(primitive->p_figure);
        free(primitive);
        previous->next_primitive = 0;
        free_primitives(previous);
        return;
    }
    free_primitives(primitive->next_primitive);
    return;
}

inline static void free_scenes(type_scene* scene)
{
    if( 0 == scene->previous_scene && 0 == scene->next_scene )
    {
        free(scene);
        space.scene = 0;
        return;
    }
    if( 0 == scene->next_scene)
    {
        type_scene* previous = scene->previous_scene;
        free_primitives(scene->p_primitive);
        free(scene);
        previous->next_scene = 0;
        free_scenes(previous);
        return;
    }
    free_scenes(scene->next_scene);
    return;
}

inline static void initialize_vertex(type_primitive* primitive, type_point point)
{
    type_point *p_temp = (type_point*)malloc(sizeof(type_point)*(primitive->len_p_figure+1));
    memcpy(p_temp, primitive->p_figure, sizeof(type_point)*(primitive->len_p_figure));

    type_point offset = get_current_scene()->axis_center;
    point.x -= offset.x;
    point.y -= offset.y;
    point.z -= offset.z;

    *(p_temp+primitive->len_p_figure) = point;

    primitive->len_p_figure++;
    free(primitive->p_figure);
    primitive->p_figure = p_temp;
    return;

}

inline static uint8_t initialize_next_primitive(type_primitive* primitive, type_point point, uint8_t type_figure)
{
    if( 0 == primitive->next_primitive)
    {
        primitive->next_primitive = (type_primitive*)malloc(sizeof(type_primitive));
        if( 0 == primitive->next_primitive ) return 0;
        primitive->next_primitive->previous_primitive = primitive;
        primitive->next_primitive->next_primitive = 0;
        primitive->next_primitive->len_p_figure = 0;
        initialize_figure(primitive->next_primitive, point, type_figure);
        if( 0 == primitive->next_primitive->p_figure ) return 0;
        return 1;
    }
    return initialize_next_primitive(primitive->next_primitive, point, type_figure);
}

inline static uint8_t initialize_next_scene(type_scene* scene, type_point point)
{
    if( 0 == scene->next_scene)
    {
        scene->next_scene = (type_scene*)malloc(sizeof(type_scene));
        if( 0 == scene->next_scene ) return 0;
        scene->next_scene->angle_x = (scene->next_scene->angle_y = (scene->next_scene->angle_z = 0));
        scene->next_scene->axis_center = point;
        scene->next_scene->previous_scene = scene;
        scene->next_scene->next_scene = 0;
        scene->next_scene->p_primitive = 0;
        scene->next_scene->current_primitive = 0;
        scene->next_scene->work_primitive = 0;
        return 1;
    }
    return initialize_next_scene(scene->next_scene, point);
}

inline static uint8_t initialize_scene(type_point point)
{
    if( 0 == space.scene )
    {
        space.scene = (type_scene*)malloc(sizeof(type_scene));
        if( 0 == space.scene ) return 0;
        space.scene->angle_x = (space.scene->angle_y = (space.scene->angle_z = 0));
        space.scene->axis_center = point;
        space.scene->previous_scene = (space.scene->next_scene = 0);
        space.scene->p_primitive = 0;
        space.scene->current_primitive = 0;
        space.scene->work_primitive = 0;
        space.current_scene = 0;
        space.work_scene = 0;
        init = 1;
        return 1;
    }
    return initialize_next_scene(space.scene, point);
}

inline static void initialize_cube(type_primitive* primitive, type_point point)
{
    primitive->p_figure = (type_point*)malloc(8*sizeof(type_point));
    type_point offset = get_current_scene()->axis_center;
    primitive->len_p_figure = 8;
    primitive->type_of_primitive = CUBE;
    offset.x = point.x - offset.x;
    offset.y = point.y - offset.y;
    offset.z = point.z - offset.z;

    for(uint8_t i = 0; i < 8; i++)
    {
        if( 0 == i || 1 == i || 2 == i || 3 == i )
        {
            ( *(primitive->p_figure + i) ).z = - (CUBE_LENGTH>>1) + offset.z;
        }
        if( 4 == i || 5 == i || 6 == i || 7 == i )
        {
            ( *(primitive->p_figure + i) ).z = (CUBE_LENGTH>>1) + offset.z;
        }

        if( 0 == i || 3 == i || 4 == i || 7 == i )
        {
            ( *(primitive->p_figure + i) ).y = - (CUBE_LENGTH>>1) + offset.y;
        }
        if( 1 == i || 2 == i || 5 == i || 6 == i )
        {
            ( *(primitive->p_figure + i) ).y = (CUBE_LENGTH>>1) + offset.y;
        }

        if( 2 == i || 3 == i || 6 == i || 7 == i )
        {
            ( *(primitive->p_figure + i) ).x = - (CUBE_LENGTH>>1) + offset.x;
        }
        if( 0 == i || 1 == i || 4 == i || 5 == i )
        {
            ( *(primitive->p_figure + i) ).x = (CUBE_LENGTH>>1) + offset.x;
        }
    }
}

inline static void initialize_figure(type_primitive* primitive, type_point point, uint8_t type)
{
    switch(type)
    {
    case CUBE:
        initialize_cube(primitive, point);
        return;
    case CUSTOM:
        if( 0 == primitive->len_p_figure)
        {
            primitive->type_of_primitive = CUSTOM;
            primitive->p_figure = (type_point*)malloc(sizeof(type_point)*0);
        }
        initialize_vertex(primitive, point);
    default:
        return;
    }
}

inline static type_primitive* get_current_primitive()
{
    if( 0 == get_current_scene()) return 0;
    if( 0 == get_current_scene()->current_primitive )
    {
        if( 0 == get_current_scene()->p_primitive ) return 0;
        return get_current_scene()->p_primitive;
    }
    uint8_t num = get_current_scene()->current_primitive;
    if( 0 == get_current_scene()->p_primitive->next_primitive ) return 0;
    return get_primitive_number(get_current_scene()->p_primitive->next_primitive, --num);
}

inline static type_primitive* get_primitive_number(type_primitive* primitive, uint8_t num)
{
    if( 0 == num ) return primitive;
    if( 0 == primitive->next_primitive ) return 0;
    return get_primitive_number(primitive->next_primitive, --num);
}

inline static type_scene* get_current_scene()
{
    if (0 == init) return 0;
    if( 0 == space.current_scene )
    {
        if( 0 == space.scene ) return 0;
        return space.scene;
    }
    uint8_t num = space.current_scene;
    if( 0 == space.scene->next_scene ) return 0;
    return get_scene_number(space.scene->next_scene, --num);
}

inline static type_scene* get_scene_number(type_scene* scene, uint8_t num)
{
    if( 0 == num) return scene;
    if( 0 == scene->next_scene ) return 0;
    return get_scene_number(scene->next_scene, --num);
}

inline static uint8_t alt_angle_scene_service(uint8_t num, uint16_t angle, uint8_t axis)
{
    type_scene* temp_scene = get_scene_number(space.scene, num);
    if( 0 == temp_scene ) return 0;

    angle = angle%360;
    switch(axis)
    {
    case AXIS_X:
        temp_scene->angle_x += angle;
        break;
    case AXIS_Y:
        temp_scene->angle_y += angle;
        break;
    case AXIS_Z:
        temp_scene->angle_z += angle;
        break;
    default:
        return 0;
    }

    register double m_sin = sin( (angle*PI)/180.0 );
    register double m_cos = cos( (angle*PI)/180.0 );

    type_primitive *temp_primitive = get_primitive_number(temp_scene->p_primitive, 0);
    while( 0 != temp_primitive)
    {
        for(uint16_t i = 0; i < temp_primitive->len_p_figure; i++)
        {
            type_point temp = *( (*temp_primitive).p_figure + i );

            switch(axis)
            {
            case AXIS_X:
                ( *(temp_primitive->p_figure+i) ).y = temp.y*m_cos - temp.z*m_sin;
                ( *(temp_primitive->p_figure+i) ).z = temp.y*m_sin + temp.z*m_cos;
                continue;
            case AXIS_Y:
                ( *(temp_primitive->p_figure+i) ).x = temp.x*m_cos + temp.z*m_sin;
                ( *(temp_primitive->p_figure+i) ).z = - temp.x*m_sin + temp.z*m_cos;
                continue;
            case AXIS_Z:
                ( *(temp_primitive->p_figure+i) ).x = temp.x*m_cos - temp.y*m_sin;
                ( *(temp_primitive->p_figure+i) ).y = temp.x*m_sin + temp.y*m_cos;
                continue;
            default:
                break;
            }
        }
        temp_primitive = temp_primitive->next_primitive;
    }
    return 1;
}

inline static void draw_custom(GtkWidget* widget, type_primitive* primitive, uint8_t num)
{
    for(uint8_t i = 0; i < primitive->len_p_figure - 1; i++)
    {
        draw_linear_interpolation(widget, primitive->p_figure+i, primitive->p_figure+i+1, num);
    }
    return;
}

inline static void draw_primitives(GtkWidget* widget, type_primitive* primitive, uint8_t num)
{
    switch(primitive->type_of_primitive)
    {
    case CUBE:
        draw_cube(widget, primitive, num);
    break;
    case CUSTOM:
        draw_custom(widget, primitive, num);
    break;
    default:;
    }
    if( 0 != primitive->next_primitive) draw_primitives(widget, primitive->next_primitive, num);
    return;
}

inline static void draw_scenes(GtkWidget* widget, type_scene* scene, uint8_t num)
{
    if( 0 != scene->p_primitive ) draw_primitives(widget, scene->p_primitive, num);
    if( 0 != scene->next_scene ) draw_scenes(widget, scene->next_scene, ++num);
    return;
}

inline static void draw_cube(GtkWidget* widget, type_primitive* primitive, uint8_t num)
{

    draw_linear_interpolation(widget, primitive->p_figure, primitive->p_figure+1, num);
    draw_linear_interpolation(widget, primitive->p_figure, primitive->p_figure+3, num);
    draw_linear_interpolation(widget, primitive->p_figure, primitive->p_figure+4, num);

    draw_linear_interpolation(widget, primitive->p_figure+2, primitive->p_figure+1, num);
    draw_linear_interpolation(widget, primitive->p_figure+2, primitive->p_figure+3, num);
    draw_linear_interpolation(widget, primitive->p_figure+2, primitive->p_figure+6, num);

    draw_linear_interpolation(widget, primitive->p_figure+5, primitive->p_figure+1, num);
    draw_linear_interpolation(widget, primitive->p_figure+5, primitive->p_figure+4, num);
    draw_linear_interpolation(widget, primitive->p_figure+5, primitive->p_figure+6, num);

    draw_linear_interpolation(widget, primitive->p_figure+7, primitive->p_figure+3, num);
    draw_linear_interpolation(widget, primitive->p_figure+7, primitive->p_figure+4, num);
    draw_linear_interpolation(widget, primitive->p_figure+7, primitive->p_figure+6, num);
    return;
}

inline static void draw_linear_interpolation(GtkWidget* widget, type_point* point_a, type_point* point_b, uint8_t num)
{
    type_point max = point_a->y > point_b->y ? *(point_a) : *(point_b);
    type_point min = point_a->y < point_b->y ? *(point_a) : *(point_b);

    register double len = max.y - min.y;
    register double step = (max.z - min.z)/len;

    if( 1 > step && -1 < step )
    {
        for(uint16_t i = 0; i < len; i++)
        {
            draw_brush(widget,
            (get_scene_number(space.scene, num)->axis_center.y + DEFAULT_Y_OFFSET) + min.y+i,
            (get_scene_number(space.scene, num)->axis_center.z + DEFAULT_Z_OFFSET) + i*step+min.z);
        }
        return;
    }

    max = point_a->z > point_b->z ? *(point_a) : *(point_b);
    min = point_a->z < point_b->z ? *(point_a) : *(point_b);

    len = max.z - min.z;
    step = (max.y - min.y)/len;

    if( 1 > step && -1 < step )
    {
        for(uint16_t i = 0; i < len; i++)
        {
            draw_brush(widget,
            (get_scene_number(space.scene, num)->axis_center.y  + DEFAULT_Y_OFFSET) + i*step+min.y,
            (get_scene_number(space.scene, num)->axis_center.z + DEFAULT_Z_OFFSET) + min.z+i);
        }
        return;
    }

    return;
}

void delete_current_primitive()
{
    delete_primitive(get_current_primitive( get_current_scene()->p_primitive ), 0);
}

void delete_current_scene()
{
    delete_scene(get_current_scene(), 0);
}

void free_all()
{
    if( 0 != space.scene ) free_scenes(space.scene);
    return;
}

uint8_t create_vertex(type_point point)
{
    if( 0 == space.scene )
    {
        create_scene(point);
        create_primitive(CUSTOM, point);
    }
    if( 0 == get_current_primitive())
    {
        create_primitive(CUSTOM, point);
    }
    type_primitive* current_primitive = get_current_primitive();
    if( 0 == current_primitive ) return 0;
    initialize_figure(current_primitive, point, CUSTOM);

}

uint8_t create_primitive(uint8_t type_figure, type_point point)
{
    if( 0 == space.scene )
    {
        create_scene(point);
    }
    type_scene* current_scene = get_current_scene();
    if ( 0 == current_scene ) return 0;
    if( 0 == current_scene->p_primitive )
    {
        current_scene->p_primitive = (type_primitive*)malloc(sizeof(type_primitive));
        if( 0 == current_scene->p_primitive ) return 0;
        current_scene->p_primitive->previous_primitive = 0;
        current_scene->p_primitive->next_primitive = 0;
        current_scene->p_primitive->type_of_primitive = 0;
        current_scene->p_primitive->len_p_figure = 0;
        if( CUSTOM != type_figure ) initialize_figure(current_scene->p_primitive, point, type_figure);
        if( 0 == current_scene->p_primitive->p_figure ) return 0;
        current_scene->current_primitive = 0;
        return 1;
    }
    return initialize_next_primitive(current_scene->p_primitive, point, type_figure);
}

void create_scene(type_point point)
{
    if( 0 == initialize_scene(point) )
    {
        free_scenes(space.scene);
        pid_t pid = getpid();
        kill(pid, SIGTERM);
    }
    return;
}

void alt_angle_scene(uint16_t angle, uint8_t axis)
{
    type_scene* temp_scene = get_current_scene();
    if( 0 == temp_scene ) return;

    angle = angle%360;

    switch(axis)
    {
    case AXIS_X:
        temp_scene->angle_x += angle;
        break;
    case AXIS_Y:
        temp_scene->angle_y += angle;
        break;
    case AXIS_Z:
        temp_scene->angle_z += angle;
        break;
    default:
        return;
    }

    register double m_sin = sin( (angle*PI)/180.0 );
    register double m_cos = cos( (angle*PI)/180.0 );

    type_primitive *temp_primitive = get_current_primitive();
    while( 0 != temp_primitive)
    {
        for(uint16_t i = 0; i < temp_primitive->len_p_figure; i++)
        {
            type_point temp = *( (*temp_primitive).p_figure + i );

            switch(axis)
            {
            case AXIS_X:
                ( *(temp_primitive->p_figure+i) ).y = temp.y*m_cos - temp.z*m_sin;
                ( *(temp_primitive->p_figure+i) ).z = temp.y*m_sin + temp.z*m_cos;
                continue;
            case AXIS_Y:
                ( *(temp_primitive->p_figure+i) ).x = temp.x*m_cos + temp.z*m_sin;
                ( *(temp_primitive->p_figure+i) ).z = - temp.x*m_sin + temp.z*m_cos;
                continue;
            case AXIS_Z:
                ( *(temp_primitive->p_figure+i) ).x = temp.x*m_cos - temp.y*m_sin;
                ( *(temp_primitive->p_figure+i) ).y = temp.x*m_sin + temp.y*m_cos;
                continue;
            default:
                break;
            }
        }
        temp_primitive = temp_primitive->next_primitive;
    }
    return;
}

void alt_all_scenes(uint16_t angle, uint8_t axis)
{
    uint8_t num = 0;
    while( 0 != alt_angle_scene_service(num++, angle, axis));
}

void draw_all(GtkWidget *widget)
{
    if( 0 != space.scene)
    {
        draw_scenes(widget, space.scene, 0);
    }
    return;
}

void mod_current_primitive(int8_t n)
{
    get_current_scene()->current_primitive += n;
}

uint8_t has_current_primitive()
{
    return get_current_primitive() == 0 ? 0 : 1;
}

void mod_current_scene(int8_t n)
{
    space.current_scene += n;
}

uint8_t has_current_scene()
{
    return get_current_scene() == 0 ? 0 : 1;
}


#endif
