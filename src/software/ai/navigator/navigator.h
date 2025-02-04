#pragma once

#include "software/ai/intent/intent.h"
#include "software/ai/intent/intent_visitor.h"
#include "software/ai/navigator/navigator.h"
#include "software/ai/navigator/obstacle/obstacle.h"
#include "software/ai/navigator/path_planner/path_planner.h"
#include "software/ai/primitive/primitive.h"
#include "software/util/parameter/dynamic_parameters.h"
#include "software/world/world.h"

/**
 * This Navigator converts the given Intents into their respective Primitives
 *
 * It will construct a path planner to navigate around AvoidAreas
 */
class Navigator : public IntentVisitor
{
   public:
    explicit Navigator(std::unique_ptr<PathPlanner> path_planner);

    std::vector<std::unique_ptr<Primitive>> getAssignedPrimitives(
        const World &world, const std::vector<std::unique_ptr<Intent>> &assignedIntents);

    std::vector<std::vector<Point>> getPlannedPaths();

    /**
     * Visits a CatchIntent to perform an operation.
     *
     * @param catch_intent The CatchIntent to visit
     */
    void visit(const CatchIntent &catch_intent) override;

    /**
     * Visits a ChipIntent to perform an operation.
     *
     * @param chip_intent The ChipIntent to visit
     */
    void visit(const ChipIntent &chip_intent) override;

    /**
     * Visits a DirectVelocityIntent to perform an operation.
     *
     * @param direct_velocity_intent The DirectVelocityIntent to visit
     */
    void visit(const DirectVelocityIntent &direct_velocity_intent) override;

    /**
     * Visits a DirectWheelsIntent to perform an operation.
     *
     * @param direct_wheels_intent The DirectWheelsIntent to visit
     */
    void visit(const DirectWheelsIntent &direct_wheels_intent) override;

    /**
     * Visits a DribbleIntent to perform an operation.
     *
     * @param dribble The DribbleIntent to visit
     */
    void visit(const DribbleIntent &dribble_intent) override;

    /**
     * Visits a KickIntent to perform an operation.
     *
     * @param kick_intent The KickIntent to visit
     */
    void visit(const KickIntent &kick_intent) override;

    /**
     * Visits a MoveIntent to perform an operation.
     *
     * @param move_intent The MoveIntent to visit
     */
    void visit(const MoveIntent &move_intent) override;

    /**
     * Visits a MoveSpinIntent to perform an operation.
     *
     * @param move_spin_intent The MoveSpinIntent to visit
     */
    void visit(const MoveSpinIntent &move_spin_intent) override;

    /**
     * Visits a PivotIntent to perform an operation.
     *
     * @param pivot_intent The PivotIntent to visit
     */
    void visit(const PivotIntent &pivot_intent) override;

    /**
     * Visits a StopIntent to perform an operation.
     *
     * @param stop_intent The StopIntent to visit
     */
    void visit(const StopIntent &stop_intent) override;

   private:
    /**
     * Create an obstacle for the given avoid area, with a buffer such that the edge
     * of the robot does not protrude into the area
     *
     * @param avoid_area The area to convert into an obstacle
     *
     * @return A obstacle representing the given area
     */
    std::optional<Obstacle> obstacleFromAvoidArea(AvoidArea avoid_area);

    // This navigators knowledge / state of the world
    World world;

    // The current Primitive the navigator has created from an Intent.
    // This variable is set by each `visit` function
    std::unique_ptr<Primitive> current_primitive;

    // The current Robot the navigator has navigated for from an Intent.
    // This variable is set by each `visit` function
    std::optional<Robot> current_robot;

    // The current destination the navigator has navigated to from an Intent.
    // This variable is set by each `visit` function
    Point current_destination;

    std::vector<Obstacle> velocity_obstacles;

    std::vector<std::vector<Point>> planned_paths;

    /**
     * Creates a list of obstacles to avoid based on avoid areas,
     * enemy team, and friendly team, while excluding the robot attached to robot_id
     *
     * @param avoid_areas specifies areas to create obstacles for
     * @param robot_id current robot that is not an obstacle
     *
     * @returns list of obstacles
     */
    std::vector<Obstacle> createCurrentObstacles(
        const std::vector<AvoidArea> &avoid_areas, unsigned int robot_id);

    /**
     * Calculates a factor for how close p is to an enemy obstacle.
     * 0 = touching or inside
     * 1 = greater than/equal to 2m away
     * scaled linearly between these values
     *
     * @param p point to evaluate
     *
     * @return A factor from 0 to 1 for how close p is to an enemy obstacle
     */
    double getCloseToEnemyObstacleFactor(const Point &p);

    /**
     * Set the current_primitive based on the intent and path
     *
     * @param move_intent MoveIntent to navigate with
     * @param path path to navigate
     *
     * @modifies current_primitive
     */
    void moveNavigation(const MoveIntent &move_intent, const Path &path);

    std::unique_ptr<PathPlanner> path_planner;
};
