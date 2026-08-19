#ifndef CONFIG_HPP
#define CONFIG_HPP

enum class AssociationMethod {
    Greedy, 
    Hungarian
};

struct TrackerConfig {
    // Max distance allowed when associating a detection
    double maxAssociationDistanceSquared = 1000.0;

    // Weight given to the previous velocity estimate
    double velocitySmoothing = 0.7;

    // Number of consecutive missed detections before deleting
    int maxMissedFrames = 3;
    
    AssociationMethod associationMethod = AssociationMethod::Hungarian;
};

#endif