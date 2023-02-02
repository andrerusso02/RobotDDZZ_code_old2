#include "ddzz_base/ddzz_hardware.h"

#include <controller_manager/controller_manager.h>
#include <ros/ros.h>


int main(int argc, char *argv[])
{
    
    // Initialize the ROS node
    ros::init(argc, argv, "ddzz_base");
    ros::NodeHandle nh;

    // Create instance of the robot
    DdzzHardware ddzz(nh);

    // Create an instance of the controller manager and pass it the robot, so that it can handle its resources.
    controller_manager::ControllerManager cm(&ddzz);

    // Pour eviter que le demarrage du controlleur bloque la boucle principale
    ros::AsyncSpinner spinner(1);
    spinner.start();

    // Hz rate
    ros::Rate rate(50.0); 

    ros::Time last_time = ros::Time::now();
    rate.sleep();
    
    
    // Blocks until shutdown signal recieved
    while (ros::ok())
    {
        
        // Read the current state of the robot from the hardware
        ddzz.readFromHardware();

        ros::Duration period = ros::Time::now() - last_time;

        // Update the controller manager
        cm.update(ros::Time::now(), period);

        // Write the commands to the robot
        ddzz.writeToHardware();

        rate.sleep();
    }

    return 0;
}