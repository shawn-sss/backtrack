#ifndef SCHEDULESERVICEMANAGERCONSTANTS_H
#define SCHEDULESERVICEMANAGERCONSTANTS_H

namespace ScheduleServiceManagerConstants {

// Timer / validation rules
inline constexpr int k_NEXT_MINUTE_STEP_SECONDS = 60;   // enforce the next :00 minute boundary
inline constexpr int k_MINIMUM_REARM_FALLBACK_MS = 60'000; // if due <= now, fallback 60s

} // namespace ScheduleServiceManagerConstants

#endif // SCHEDULESERVICEMANAGERCONSTANTS_H
