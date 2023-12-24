#include "KeyboardMovementController.h"


namespace lve
{
    void KeyboardMovementController::moveInPlaneXZ(
        GLFWwindow* window,
        float dt,
        GameObject& obj)
    {
        glm::vec3 rotate{ 0.0f };
        if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate.y += 1.0f;
        if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.0f;
        if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate.x += 1.0f;
        if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate.x -= 1.0f;

        if (glm::dot(rotate, rotate) > glm::epsilon<float>())
        {
            obj.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
        }

        obj.transform.rotation.x = glm::clamp(obj.transform.rotation.x, -1.5f, 1.5f);
        obj.transform.rotation.y = glm::mod(obj.transform.rotation.y, glm::two_pi<float>());

        float yaw = obj.transform.rotation.y;
        const glm::vec3 forwardDir{ sin(yaw), 0.0f, glm::cos(yaw) };
        const glm::vec3 rightDir{ forwardDir.z, 0.0f, -forwardDir.x };
        const glm::vec3 upDir{ 0.0f, -1.0f, 0.0f };

        glm::vec3 moveDir{ 0.0f };
        if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
        if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
        if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
        if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
        if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += upDir;
        if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

        if (glm::dot(moveDir, moveDir) > glm::epsilon<float>())
        {
            obj.transform.translation += moveSpeed * dt * glm::normalize(moveDir);
        }
    }
}