#ifndef SCHEDULESERVICEMANAGERCONSTANTS_H
#define SCHEDULESERVICEMANAGERCONSTANTS_H

// Validation and timing rules
namespace ScheduleServiceManagerConstants {
inline constexpr int NextMinuteStepSeconds = 60;
inline constexpr int MinimumRearmFallbackMs = 60'000;
}

// Keys for schedule node in user settings
namespace SchedKeys {
inline constexpr char Node[] = "schedule";
inline constexpr char Enabled[] = "enabled";
inline constexpr char WhenIso[] = "when_iso";
inline constexpr char Recur[] = "recur";
inline constexpr char LegacyEnabled[] = "schedule_enabled";
inline constexpr char LegacyWhenIso[] = "schedule_when_iso";
inline constexpr char LegacyRecur[] = "schedule_recur";
}

#endif
