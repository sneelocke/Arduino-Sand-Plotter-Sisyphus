10/21/17 Saturday
sandart - first experiments.

10/22/17 Sunday
nine-flower - make a 9 pedal flower, shrinking
erase - simple erase program
sweeptest - first program to sweep the arm
six-flower - make 6 pedal flower, shrinking
checkerboard - make neat checkerboard pattern
ten-degrees - make ticks every ten degrees
squarewave - shrinking 36 tooth gear
servo-cal - sweep back and forth to calibrate servo.

10/23/17 Monday
figure how to more accurately position the arm with respect to real time.
call with servo.writeMicroseconds() insead of servo.write().
call with 1000->1250 to stay on yellow plate.  This gives
250 positions on the plate from center to edge.  This really improves things.

sweeptest2 - used to determine max sweep of servo with new control calls.
nine-flower2 - really nice erase with 9 lobe flower.
six-flower2 - really nice erase with 6 lobe flower.
spiral2 - nice erase with spiral - not very good
spiral2b - erase with thin spiral.  long plot.
checkerboard2 - reducing checkerboard.

10/24/17 Tuesday
polar2 - several polar plots, redesigned code for time based rotation
NEW LARGER sand table installed, 7 & 3/8 diameter.

servo-cal used to glue down servo.
polar2 works well, now rewrite others for larger table.
added checkerboard2 to polar2, and renamed it to Sand_art.

10/25/17 Wednesday
wrote ruffle2, which produces nice art
wrote sand_art_rand, which uses only ruffle2 converted to ruffle_rand
   to make random ruffle designs.  

10/26/17 Thursday
rewrote sand_art_rand to make maximum use of table, small drawings
are not wanted.

10/27/17 Friday
taught it straight lines to draw rotating triangles, squares
6 point star, and 8 point stars.  Took basic program to make the
necessary tables.

10/28/17 Saturday
Wrote N sided general purpose polygon maker routine.
Going to need some tweeking, maybe best to continually down scale
not just scale down at the end of each rotation.
Sand is very wet since AC has been off for two days.

10/29/17 Sunday
Wrote roof shingles and negative roof shingles routine.
Looks great.

10/30/17 Monday
Wrote odd pointed star routines.

10/31/17 Tuesday
Wrote circle plot routines.

11/1/17 Wednesday
Wrote outside spirograph routines.
And inside spirograph routines.

11/6/17 Monday
Spirograph routines can't work.  They need to be pre-processed to produce
a table of R vs Theta values.  These can be plotted when the table is in position.

11/11/17 Saturday
Wrote Cardioid routine and inverse Cardioid routine.  Like the latter.
Made headers to solder wires in order to stabilize the setup.  Sifted and dried
sand most of the afternoon.  Now have enough for the Makerfaire.

11/13/17 Monday
Added a case statement driven on a board jumper.  Tried to write the hyperbolic
tile routine.  NG there.  

11/14/17 Tuesday
Wrote the nice shifted sine routine.  Everything looks ready for Makerfaire.

11/19/17 Sunday
Makerfaire went well.  Lighting rotten.  Lynn loaned a flashlight that made it work.
Wrote fine spiral erase.  Experimented with negative servo positions

2/5/18 Monday
Strange dislinearity in some plots was a programming error.   If you reduce the scale
after each rotation, and allow it to spiral, eventually most plots will cross the
line where the scale changes, causing a bump in the drawing.  FIX- only reduce the
scale per iteration, not per rotation. 



TO DO-
piezo chirp at start of showpage
replace drive motor with one that won't get hot.  ALL GET HOT EVEN WHEN NEW.



