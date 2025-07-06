# Image Command Interpreter - Matrix Image Editor

A C-based interpreter that processes single-character commands to generate PPM images using layer stacks, drawing operations, and flood fill algorithms.

## Project Structure

```
.
├── projet.c           # Main implementation file
├── projet.h           # Header file with structures and function declarations
├── Makefile          # Build configuration
├── prog              # Compiled executable
├── *.ppm             # Sample PPM image files for testing
├── ok.ipi            # Sample input command file
├── README.md         # This documentation
└── .vscode/          # Visual Studio Code configuration
    ├── c_cpp_properties.json
    └── settings.json
```

## Core Data Structures

### Image Management
- **[`image`](projet.h)**: Square matrix of pixels with size information
- **[`pixel`](projet.h)**: Individual pixel containing RGB color and opacity
- **[`couleur`](projet.h)**: RGB color structure (Red, Green, Blue: 0-255)
- **[`opacite`](projet.h)**: Opacity structure (0-255)

### Layer System
- **[`pile_calque`](projet.h)**: Stack containing up to 10 image layers
- **[`position`](projet.h)**: Current and marked positions with direction (North, East, South, West)

### Color Management
- **[`sceau_de_couleur`](projet.h)**: Linked list storing colors for drawing operations
- **[`sceau_opacite`](projet.h)**: Linked list storing opacity values for drawing operations

### Utility Structures
- **[`pile_pos`](projet.h)**: Position stack used for flood fill algorithm
- **[`contenu_ligne`](projet.h)**: Line content structure for input parsing

## Key Features

### Layer Operations
- **Stack-based management**: Images organized in a stack with max 10 layers (defined by [`Taille`](projet.h))
- **Layer fusion**: Combine layers using [`fusion_calque`](projet.c) with alpha blending
- **Layer masking**: Cut layers using masks with [`decoupage_calque`](projet.c)

### Drawing Operations
- **Line drawing**: Draw lines between marked and current positions using [`trace_ligne`](projet.c)
- **Flood fill**: Fill connected areas with [`remplissage`](projet.c)
- **Position tracking**: Navigate with current and marked positions using [`position`](projet.h)

### Color System
- **Color buckets**: Maintain linked lists of colors and opacities
- **Average calculation**: Compute average colors/opacities using [`moyenne_sceau_couleur`](projet.c) and [`moyenne_sceau_opacite`](projet.c)
- **Transparency support**: Full opacity channel with alpha blending in [`calcul_pixel_courant`](projet.c)

## Command System

The program reads single-character commands from stdin through [`main`](projet.c):

### Color Commands
- `n`: Add black (0,0,0)
- `r`: Add red (255,0,0)
- `g`: Add green (0,255,0)
- `b`: Add blue (0,0,255)
- `y`: Add yellow (255,255,0)
- `m`: Add magenta (255,0,255)
- `c`: Add cyan (0,255,255)
- `w`: Add white (255,255,255)

### Opacity Commands
- `t`: Add transparent (opacity 0)
- `o`: Add opaque (opacity 255)

### Movement Commands
- `v`: Move forward in current direction with wraparound
- `h`: Turn right (clockwise)
- `a`: Turn left (counter-clockwise)

### Drawing Commands
- `p`: Mark current position using [`mise_a_jour_position_marquee`](projet.c)
- `l`: Draw line from marked to current position
- `f`: Flood fill current area
- `i`: Clear color and opacity buckets

### Layer Commands
- `s`: Add new layer to stack
- `e`: Merge top two layers
- `j`: Cut layer using mask

## Build Instructions

### Compilation
Use the provided [`Makefile`](Makefile):
```bash
make
```

### Clean Build Files
```bash
make clean
```

## Usage

### Input Format
1. First line: Image size (e.g., "100" for 100x100 image)
2. Following lines: Command sequences

### Execution
```bash
./prog < input_file
```

### Output
The program outputs the final image in PPM P6 format to stdout using [`afficher_image`](projet.c).

### Example
```bash
./prog < ok.ipi > output.ppm
```

## Core Functions

### Image Management
- [`creer_image`](projet.c): Create new image with specified dimensions
- [`afficher_image`](projet.c): Output image in PPM P6 format
- [`free_image`](projet.c): Free image memory

### Layer Operations
- [`ajouter_image_pile_calque`](projet.c): Add image to layer stack
- [`depiler_image_pile_calque`](projet.c): Remove and return top layer
- [`fusion_calque`](projet.c): Merge two top layers with alpha blending formula
- [`decoupage_calque`](projet.c): Apply mask to layer using opacity multiplication

### Drawing Functions
- [`trace_ligne`](projet.c): Draw line using digital differential analyzer (DDA) algorithm
- [`remplissage`](projet.c): Flood fill implementation using 4-connected neighbors
- [`calcul_pixel_courant`](projet.c): Calculate current pixel color/opacity from buckets

### Color Bucket Management
- [`ajouter_sceau_de_couleur`](projet.c): Add color to linked list bucket
- [`ajouter_sceau_opacite`](projet.c): Add opacity to linked list bucket
- [`vider_sceau_couleur`](projet.c): Clear and free color bucket
- [`vider_sceau_opacite`](projet.c): Clear and free opacity bucket

### Position Management
- [`init_position`](projet.c): Initialize position to (0,0) facing East
- [`empiler_pile_pos`](projet.c): Push position to stack
- [`depiler_pile_pos`](projet.c): Pop position from stack

## Algorithm Details

### Line Drawing
The [`trace_ligne`](projet.c) function implements a digital differential analyzer:
- Calculates delta between marked (`pos.marquee`) and current (`pos.courante`) positions
- Uses [`max`](projet.c) function to determine line length
- Draws pixels along the line path with proper scaling

### Flood Fill
The [`remplissage`](projet.c) function uses a stack-based approach:
- Maintains a history matrix to avoid infinite loops
- Processes 4-connected neighbors (North, South, East, West)
- Fills areas with matching color and opacity values

### Layer Fusion
The [`fusion_calque`](projet.c) function implements alpha blending:
- Formula: `result = background + (foreground * (255 - alpha) / 255)`
- Combines RGB channels and opacity independently
- Preserves transparency information

### Movement System
Position updates in [`main`](projet.c) handle wraparound:
- East/West: wraps at image boundaries horizontally
- North/South: wraps at image boundaries vertically
- Direction changes: `h` (right turn), `a` (left turn)

## Memory Management

Comprehensive memory management throughout:
- [`free_image`](projet.c): Free pixel matrices
- [`free_pile_pos`](projet.c): Free position stack
- [`vider_sceau_couleur`](projet.c): Clear and free color buckets
- [`vider_sceau_opacite`](projet.c): Clear and free opacity buckets

## Error Handling

- Boundary checking for image coordinates
- Stack overflow/underflow protection in [`empiler_pile_pos`](projet.c) and [`depiler_pile_pos`](projet.c)
- Proper memory deallocation on program exit
- Command validation in switch statement

## Sample Files

- **PPM files**: Test images ([`base.ppm`](base.ppm), [`best.ppm`](best.ppm), [`simple.ppm`](simple.ppm), etc.)
- **Command files**: Input sequences ([`ok.ipi`](ok.ipi))

## Technical Specifications

- **Maximum layers**: 10 (defined by [`Taille`](projet.h) constant)
- **Image format**: Square matrices only
- **Color depth**: 8 bits per channel (RGB + opacity)
- **Output format**: PPM P6 binary format
- **Position system**: 0-indexed coordinates with directional movement

## Input/Output Examples

### Basic Drawing Sequence
```
100          # Create 100x100 image
rg           # Add red and green to color bucket
o            # Add opaque to opacity bucket
p            # Mark current position (0,0)
vvv          # Move 3 steps east to (0,3)
l            # Draw line from (0,0) to (0,3)
```

### Layer Operations
```
s            # Add new layer
wb           # Add white and blue to color bucket
f            # Flood fill current area
e            # Merge with previous layer
```

---

For detailed implementation, see [`projet.c`](projet.c) and [`projet.h`](projet.h).