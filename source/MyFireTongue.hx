import firetongue.FireTongue;
import flixel.addons.ui.interfaces.IFireTongue;

/**
 * This class does nothing.
 * It is here merely to make "Flixel UI" and "Firetongue"
 * work together.
 *
 * These are the notes from the "Flixel UI" author himself:
 *
 *     This is a simple wrapper class to solve a dilemma:
 *
 *     I don't want flixel-ui to depend on firetongue
 *     I don't want firetongue to depend on flixel-ui
 *
 *     I can solve this by using an interface, IFireTongue, in flixel-ui
 *     However, that interface has to go in one namespace or the other and if I put
 *     it in firetongue, then there's a dependency. And vice-versa.
 *
 *     This is solved by making a simple wrapper class in the actual project
 *     code that includes both libraries.
 *
 *     The wrapper extends FireTongue, and implements IFireTongue
 *
 *     The actual extended class does nothing, it just falls through to FireTongue.
 *
 */
class MyFireTongue extends FireTongue implements IFireTongue
{
	public function new()
	{
		super();
	}
}

