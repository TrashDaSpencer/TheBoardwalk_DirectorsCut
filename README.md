# TheBoardwalk_DirectorsCut

# The Boardwalk: Director's Cut
**Game Design Document — Version 0.2**

| Category | Description |
|---|---|
| Genre | 2.5D Horror / Puzzle / Adventure |
| Theme | Director's Cut |
| Visual Style | 2D sprites in a 2.5D side-scrolling environment |
| Setting | A beachfront amusement park |
| Inspiration | Santa Cruz Beach Boardwalk, California |
| Player | Single-player |
| Primary Objective | Find and rescue the protagonist's kidnapped girlfriend |
| Target Scope | Short game jam experience |

---

## 1. Game Overview

**The Boardwalk: Director's Cut** is a side-scrolling horror puzzle adventure set in an amusement park that has two versions of reality.

In the **Theatrical Release**, the park is a cheerful, bustling attraction filled with smiling mascots, entertaining clowns, and happy visitors.

In the **Director's Cut**, the park becomes a nighttime nightmare. The mascots are flesh-eating zombies, the clowns are deranged killers, and the park's hidden areas reveal a darker reality.

Players can switch between these two versions to navigate obstacles, avoid threats, access hidden areas, and search for the protagonist's missing girlfriend.

### Core Mechanic

**Switch between two versions of the same environment to solve traversal puzzles.**

A blocked path in one version may be accessible in the other. However, both versions introduce their own dangers and restrictions.

---

## 2. Opening Cinematic

### Scene 1 — Two Televisions

The game opens on a black screen.

Two old-fashioned televisions illuminate the darkness, sitting side by side.

- **Left TV:** The Theatrical Release
- **Right TV:** The Director's Cut

Both televisions begin playing different introductions to the same story.

### Left TV — Theatrical Release

*Daytime. Bright colors. Cheerful amusement park music.*

1. The protagonist arrives at the amusement park with his girlfriend for a date.
2. They enter the park together, surrounded by happy visitors and colorful attractions.
3. The protagonist excuses himself to use the bathroom.
4. While he's inside, several friendly-looking park mascots approach his girlfriend.
5. The mascots suddenly kidnap her and carry her away.
6. The protagonist exits the bathroom, looking for her.
7. He discovers her phone lying on the ground.

### Right TV — Director's Cut

*Midnight. Dark skies. An empty parking lot. Distorted carnival music.*

1. The protagonist drives up to the amusement park after hours.
2. He parks and looks toward the darkened attractions.
3. He reaches into his pocket and pulls out a folded note.
4. He unfolds it, revealing a message written in blood:

> We have your girl.  
> The boardwalk @ Midnight

5. He looks toward the entrance of the abandoned park.

### Scene 2 — Enter the Game

Both introductions finish.

The camera slowly moves toward the **left television — the theatrical release**.

The television image grows until it fills the entire screen.

The television border disappears, and the player is now inside the daytime amusement park.

**Gameplay begins immediately after the protagonist discovers his girlfriend's phone.**

The initial objective appears:

**Find your girlfriend.**

*Story note:* The two introductions deliberately present different circumstances surrounding the kidnapping. Whether one represents the truth, an alternate timeline, or a manipulated version of events remains unresolved.

---

## 3. Gameplay Mechanics

### Prototype PC Controls

| Action | Key |
|---|---|
| Move left / right | A / D |
| Jump | Space |
| Kick | Left Ctrl |
| Interact with the nearest nearby object | E |
| Switch between Theatrical Release and Director's Cut | Tab |

The prototype player uses a side-on camera and a simple primitive stand-in. Level actors can implement the `Boardwalk Interactable` interface to respond to interaction or kicks, and the `Boardwalk World Variant` interface to update when the active cut changes.

### 3.1 Switching Between Cuts

The player can switch between the Theatrical Release (Day) and the Director's Cut (Night) at any time when standing in a shared area.

Switching changes:
- Environmental appearance and lighting
- NPC behavior and enemy threats
- Accessibility of doors and pathways
- Crowds and obstacles
- Active and inactive attractions
- Background music and ambient sounds

The player's physical position remains the same, subject to collision safety.

### 3.2 Location Restrictions

Not every location exists in both cuts.

| Area Type | Switching Rule | Examples |
|---|---|---|
| Shared | Switch freely | Main pathways, open plazas, ride queues |
| Day-only | Locked to daytime | Shops, stores, food court, ride interiors |
| Night-only | Locked to nighttime | Closets, manager's office, basement, rooftops |

Once inside a restricted area, the player must return to a shared area before switching.

### 3.3 Environmental Obstacles

**Locked Doors**

Doors locked during the day may be broken open at night.

**Crowds**

Long lines of visitors obstruct pathways during the day. At night, these pathways become empty.

**Darkness**

Some nighttime areas are too dark to navigate safely. The player must use the daytime version instead.

**Functional Rides**

Certain attractions operate only during the day and can transport the player to otherwise unreachable areas.

**Structural Damage**

Destroyed barriers, exposed ladders, or broken structures may provide new pathways at night.

---

## 4. Characters and Enemies

### The Protagonist

An ordinary young man visiting the amusement park with his girlfriend.

His main motivation is finding and rescuing her.

### The Girlfriend

Kidnapped by the park's mascots at the start of the game.

Her disappearance drives the narrative. Her phone is the first clue.

### Mascots

**Daytime**
- Cheerful and friendly
- Approach visitors for hugs
- Can obstruct the player's movement

**Nighttime**
- Become horrifying zombie versions of themselves
- Chase the player
- Attempt to eat the player
- Can be avoided through movement, hiding, or switching cuts

### Clown

**Daytime**
- Entertains guests
- Uses a squirting flower to spray water
- Primarily a humorous environmental character

**Nighttime**
- Becomes a demented, murderous clown
- The flower becomes a flamethrower
- Attacks within a directional cone
- Forces the player to use cover, timing, or alternate routes

### Visitors

**Daytime:** Fill the park, queue for rides, and create obstacles.

**Nighttime:** Disappear, leaving the park eerily empty except for hostile characters.

---

## 5. World and Level Design

### Map Inspiration

The environment is inspired by the Santa Cruz Beach Boardwalk.

Key visual elements include:
- The beachfront promenade
- A large wooden roller coaster
- Carousel and carnival rides
- Arcades and game booths
- Food stands and souvenir shops
- Restrooms and service areas
- Parking lots and rear entrances
- Rooftops and maintenance passages

The game should capture the recognizable feeling of a California beachfront amusement park without requiring a geographically exact recreation.

### 2.5D Presentation

The game will use 2D sprite artwork presented in a world with visual depth.

**Visual Composition**
- Foreground: Props, fences, booths, decorative objects
- Gameplay plane: Player, enemies, interactable objects
- Background: Rides, buildings, roller coaster, beach, ocean
- Distant parallax: Sky, clouds, horizon, distant coastline

Gameplay primarily moves left and right, with jumping and vertical traversal where needed.

### Example Level Progression

**1. Main Boardwalk — Shared**

The protagonist finds his girlfriend's phone and begins exploring. The player learns to switch cuts.

**2. Ride Queue — Shared**

A massive daytime crowd blocks progress. Switching to night clears the path but introduces zombie mascots.

**3. Maintenance Closet — Night-only**

A locked daytime door is smashed open at night. The player explores the closet and discovers a clue.

**4. Food Court — Day-only**

The player must return to daytime to access the food court. It provides a safer location for exploration.

**5. Clown's Midway — Shared**

The clown squirts water during the day but attacks with a flamethrower at night. The player must determine a safe route.

**6. Roller Coaster — Day-only**

A functioning ride helps the player reach another part of the park.

**7. Rooftops — Night-only**

The player traverses maintenance ladders, damaged structures, and rooftops to reach a restricted section.

**8. Manager's Office — Night-only**

The player discovers information pointing toward his girlfriend's location.

The final rescue location and ending remain to be designed.

---

## 6. Art and Sound Direction

### Theatrical Release

**Visuals**
- Warm sunlight
- Bright, saturated colors
- Clean attractions
- Cheerful character animations
- Crowded walkways

**Sound**
- Upbeat carnival music
- Happy visitors
- Ride machinery
- Arcade noises
- Friendly mascot voices

### Director's Cut

**Visuals**
- Moonlight and dramatic shadows
- Flickering neon signs
- Damaged structures
- Bloodstains and decay
- Distorted character sprites
- Empty walkways

**Sound**
- Distorted versions of daytime music
- Ocean waves and wind
- Distant mechanical noises
- Unsettling laughter
- Zombie growls
- Flamethrower effects

### Switching Transition

A brief cinematic effect reinforces the film-editing theme.

Suggested approach:
- One or two frames of film static
- An audible film-splice sound
- Immediate environment and character transformation
- A short musical transition

The switch should remain fast enough to use during traversal and enemy encounters.

---

## 7. Player Controls and Systems

**Basic Controls**
- Move left/right
- Jump
- Interact
- Switch cuts

**Potential Additional Controls**
- Sprint
- Crouch or hide
- View clues/inventory

### Failure and Checkpoints

For the initial version:
- Hostile enemies cause damage or a checkpoint reset.
- No combat system is required.
- Checkpoints appear after major challenges.
- Switching cannot trap the player inside solid objects.
- Restricted areas must always provide an accessible return route.

---

## 8. Game Jam MVP

### Required Features

1. Opening two-TV cinematic
2. Protagonist movement and jumping
3. 2.5D side-scrolling environment
4. Day/night switching
5. Day-only and night-only areas
6. Day/night environmental differences
7. Mascot with friendly and hostile behavior
8. Clown with water/flamethrower behavior
9. Crowd-blocking puzzle
10. Broken-door puzzle
11. Small, interconnected boardwalk level
12. Objective and playable ending

### Stretch Goals

- Fully functional moving rides
- Additional mascot enemies
- More complex puzzles
- Collectible story clues
- Secret rooms
- Multiple endings
- Advanced enemy AI
- Elaborate cut-switching animations
- Larger park exploration

---

## 9. Design Guidelines

**The two cuts must feel fundamentally different.**

Night should not simply be daytime with darker lighting. It must meaningfully change how the player navigates the environment.

**Every obstacle should communicate its logic.**

Players should be able to recognize why an area is inaccessible and understand how switching might help.

**Neither cut should always be preferable.**

Daytime provides visibility, safety, and operational attractions but suffers from crowds and locked areas.

Nighttime opens restricted paths and removes crowds but introduces darkness, dangerous enemies, and environmental hazards.

**The story should support the mechanic.**

The different introductions, altered environments, and hidden areas should reinforce the mystery of why two versions of the amusement park exist.

---

## 10. Open Design Decisions

- How does the protagonist gain the ability to switch cuts?
- Does his girlfriend exist in both versions?
- Are the two opening cinematics alternate timelines or different presentations of the same events?
- Does switching pause or reset enemy behavior?
- Is there a fixed number of hit points, or does enemy contact reset the player?
- Where is the girlfriend being held?
- What is the final reveal about the park?

These can remain undecided while the foundational gameplay is prototyped.

---

**Core Identity:** A horror adventure about a man searching for his kidnapped girlfriend inside two conflicting versions of a beachfront amusement park, using the differences between the theatrical release and the director's cut to survive and uncover the truth.
