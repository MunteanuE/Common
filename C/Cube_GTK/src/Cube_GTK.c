#include "Cube_GTK.h"
#ifdef CUBE_GTK_H

static type_space space;

static void delete_primitive(type_primitive* primitive, uint8_t number)
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

static void delete_scene(type_scene* scene, uint8_t number)
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

static void free_primitives(type_primitive* primitive)
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

static void free_scenes(type_scene* scene)
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

static uint8_t initialize_next_primitive(type_primitive* primitive, type_point point, uint8_t type_figure)
{
    if( 0 == primitive->next_primitive)
    {
        primitive->next_primitive = (type_primitive*)malloc(sizeof(type_primitive));
        if( 0 == primitive->next_primitive ) return 0;
        primitive->next_primitive->previous_primitive = primitive;
        primitive->next_primitive->next_primitive = 0;
        initialize_figure(primitive->next_primitive, point, type_figure);
        if( 0 == primitive->next_primitive->p_figure ) return 0;
        return 1;
    }
    return initialize_next_primitive(primitive->next_primitive, point, type_figure);
}

static uint8_t initialize_next_scene(type_scene* scene, type_point point)
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

static uint8_t initialize_scene(type_point point)
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
        return 1;
    }
    return initialize_next_scene(space.scene, point);
}

static void initialize_cube(type_primitive* primitive, type_point point)
{
    primitive->p_figure = (type_point*)malloc(12*CUBE_LENGTH*sizeof(type_point));
    type_point offset = get_current_scene()->axis_center;
    offset.x = point.x - offset.x;
    offset.y = point.y - offset.y;
    offset.z = point.z - offset.z;

    for(uint8_t i = 0; i < 12; i++)
    {
        for(uint16_t c = 0; c < CUBE_LENGTH; c++)
        {
            if( 0 == i || 2 == i || 4 == i || 6 == i ) /* 0,2 - closed horizontals, 4,6 - far horizontals */
            {
                ( *(primitive->p_figure + i*CUBE_LENGTH + c) ).y = c + ( -(CUBE_LENGTH>>1) + offset.y);
            }
            if( 3 == i || 7 == i || 8 == i || 11 == i ) /* 3,7 - left verticales, 8, 11 - left "bridges" */
            {
                ( *(primitive->p_figure + i*CUBE_LENGTH + c) ).y = - (CUBE_LENGTH>>1) + offset.y;
            }
            if( 1 == i || 5 == i || 9 == i || 10 == i ) /* 1,5 - right verticales, 9, 11 - rihgt "bridges" */
            {
                ( *(primitive->p_figure + i*CUBE_LENGTH + c) ).y = (CUBE_LENGTH>>1) + offset.y;
            }

            if( 0 == i || 4 == i || 8 == i || 9 == i )
            {
                ( *(primitive->p_figure + i*CUBE_LENGTH + c) ).z = - (CUBE_LENGTH>>1) + offset.z;
            }
            if( 1 == i || 3 == i || 5 == i || 7 == i )
            {
                ( *(primitive->p_figure + i*CUBE_LENGTH + c) ).z = c + ( -(CUBE_LENGTH>>1) + offset.z);
            }
            if( 2 == i || 6 == i || 10 == i || 11 == i )
            {
                ( *(primitive->p_figure + i*CUBE_LENGTH + c) ).z = (CUBE_LENGTH>>1) + offset.z;
            }

            if( 0 == i || 1 == i || 2 == i || 3 == i )
            {
                ( *(primitive->p_figure + i*CUBE_LENGTH + c) ).x = - (CUBE_LENGTH>>1) + offset.x;
            }
            if( 8 == i || 9 == i || 10 == i || 11 == i )
            {
                ( *(primitive->p_figure + i*CUBE_LENGTH + c) ).x = c + ( -(CUBE_LENGTH>>1) + offset.x);
            }
            if(4==i || 5==i || 6==i || 7==i)
            {
                ( *(primitive->p_figure + i*CUBE_LENGTH + c) ).x = (CUBE_LENGTH>>1) + offset.x;
            }
        }
    }
}

static void initialize_figure(type_primitive* primitive, type_point point, uint8_t type)
{
    switch(type)
    {
    case 0:
        initialize_cube(primitive, point);
        return;
    default:
        return;
    }
}

static type_primitive* get_current_primitive()
{
    if( 0 == get_current_scene()->current_primitive )
    {
        if( 0 == get_current_scene()->p_primitive ) return 0;
        return get_current_scene()->p_primitive;
    }
    uint8_t num = get_current_scene()->current_primitive;
    if( 0 == get_current_scene()->p_primitive->next_primitive ) return 0;
    return get_primitive_number(get_current_scene()->p_primitive->next_primitive, --num);
}

static type_primitive* get_primitive_number(type_primitive* primitive, uint8_t num)
{
    if( 0 == num ) return primitive;
    if( 0 == primitive->next_primitive ) return 0;
    return get_primitive_number(primitive->next_primitive, --num);
}

static type_scene* get_current_scene()
{
    if( 0 == space.current_scene )
    {
        if( 0 == space.scene ) return 0;
        return space.scene;
    }
    uint8_t num = space.current_scene;
    if( 0 == space.scene->next_scene ) return 0;
    return get_scene_number(space.scene->next_scene, --num);
}

static type_scene* get_scene_number(type_scene* scene, uint8_t num)
{
    if( 0 == num) return scene;
    if( 0 == scene->next_scene ) return 0;
    return get_scene_number(scene->next_scene, --num);
}

static uint8_t alt_angle_scene_service(uint8_t num, uint16_t angle, uint8_t axis)
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
        for(uint16_t i = 0; i < 12*CUBE_LENGTH; i++)
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

static void draw_primitives(GtkWidget *widget, type_primitive *primitive, uint8_t num)
{
    for(uint16_t i=0; i<12*CUBE_LENGTH; i++)
    {
        draw_brush(widget,
        get_scene_number(space.scene, num)->axis_center.y + ( *(primitive->p_figure+i) ).y + DEFAULT_Y_OFFSET,
        get_scene_number(space.scene, num)->axis_center.z + ( *(primitive->p_figure+i) ).z + DEFAULT_Z_OFFSET);
    }
    if( 0 != primitive->next_primitive) draw_primitives(widget, primitive->next_primitive, num);
    return;
}

static void draw_scenes(GtkWidget *widget, type_scene *scene, uint8_t num)
{
    if( 0 != scene->p_primitive ) draw_primitives(widget, scene->p_primitive, num);
    if( 0 != scene->next_scene ) draw_scenes(widget, scene->next_scene, ++num);
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
        initialize_figure(current_scene->p_primitive, point, type_figure);
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
        for(uint16_t i = 0; i < 12*CUBE_LENGTH; i++)
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
