#include "camera.h"
#include "matrix.h"

static camera_t camera = {
    .position = { 0, 0, 0 },
    .direction = { 0, 0, 1 },
    .forward_velocity = { 0, 0, 0 },
    .yaw = 0.0
};

vec3_t get_camera_position(void) {
    return camera.position;
}
vec3_t get_camera_direction(void) {
    return camera.direction;
}
vec3_t get_camera_forward_velocity(void) {
    return camera.forward_velocity;
}
float get_camera_yaw(void) {
    return camera.yaw;
}
float get_camera_pitch(void) {
    return camera.pitch;
}
void update_camera_position(vec3_t position) {
    camera.position = position;
}
void update_camera_direction(vec3_t direction) {
    camera.direction = direction;
}
void update_camera_forward_velocity(vec3_t velocity) {
    camera.forward_velocity = velocity;
}
void rotate_camera_yaw(float angle) {
    camera.yaw += angle;
}

void rotate_camera_pitch(float angle) {
    camera.pitch += angle;
}

vec3_t get_camera_lookat_target(void) {
    //Initialize target looking at positive z-axis.
    vec3_t target = {0, 0, 1};

    //Create camera rotation matrix based on yaw, pitch and roll.
    mat4_t camera_rotation = mat4_identity();
    mat4_t camera_yaw_rotation = mat4_make_rotation_y(camera.yaw);
    mat4_t camera_pitch_rotation = mat4_make_rotation_x(camera.pitch);
    camera_rotation = mat4_mul_mat4(camera_yaw_rotation, camera_rotation);
    camera_rotation = mat4_mul_mat4(camera_pitch_rotation, camera_rotation);

    //Update camera direction based on the rotation
    vec4_t camera_direction = mat4_mul_vec4(camera_rotation, vec4_from_vec3(target));
    camera.direction = vec3_from_vec4(camera_direction);

    //Offset the camera position in the direction where the camera is pointing at
    target = vec3_add(camera.position, camera.direction);

    return target;
}
