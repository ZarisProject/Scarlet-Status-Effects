# Scarlet Status Effects
An UE5 plugin with a simple but flexible status effect system

**Status: In development**

---
### General
**Status Effects** are temporary modifiers that can be applied to any entity that has `StatusEffectsComponent` actor component.

Any number of unique status effects can be applied to the same entity at the same time. Application of status effects of the same time results in summation of their durations.

---
### Implementation

#### Status Effect Component
Every actor that can have status effects applied to it must have a `StatusEffectsComponent` (Derived from `UActorComponent`). This component stores the list of currently applied status effects, including the following information:
* `StatusEffectName` (Key Value) - ID name of the status effect;
* `TimeRemaining` - time before status effect will be removed (in seconds);
* `StatusEffectObject` - pointer to the corresponding `StatusEffect` (Derived from `UObject`).
#### Status Effect (`UObject`)
Status effect logic is implemented inside of `StatusEffect` objects (Derived from `UObject`). They implement the following functionality:
* `OnApplied` - called when the status effect is applied the first time;
* `OnReapplied` - called when the same kind of status effect is applied again (when current one is still active);
* `OnTick` - called every tick while the status effect is still active;
* `OnTimeOver` - called when status effect application time (`TimeRemaining`) runs out;
* `OnForcefullyRemoved` - called when status effect is removed by force (in any way besides `TimeRemaining` running out).

**Status effect objects can be of 2 types:**

**1. Dynamic -** a `StatusEffectDynamic` object is created separately for every actor that has it applied to it. This object is then deleted once the status effect is removed. This type of status effects is *bad for performance* but can be useful for status effects that have to store a lot of things per applicant.

**2. Static -** `StatusEffectStatic` object is created once and then used by every actor that has this status effect applied to it. *Good for performance*. Per applicant data can still be stored inside `TMap`-s that use applicant's identifiers as keys.

### Status Effect Info & Data Tables
Status Effect *Meta Information* is stored inside Data Tables of `StatusEffectInfo` structure. This meta information contains the following entries:
* `StatusEffectName` (Key Value) - ID name of the status effect;
* `StatusEffectObjectClass` - class of the object that needs to be created.
* *Additional information for GUI displaying.*

---
### Code Design




