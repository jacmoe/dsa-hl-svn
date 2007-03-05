require 'util/vector_util.rb'
require 'testcase.rb'
require 'door.rb'
require 'switch.rb'

class DoorOpener < ObjectStateChangeListener

  def initialize(door, switch)
    super()
    @door = door
    @switch = switch
    @switch.addObjectStateChangeListener(self)
  end

  def objectStateChanged(event)
      doorOpen = @switch.getState() == Switch3Way::STATE_OBEN
      if (@door.isOpen() != doorOpen)
        if (doorOpen)
          @door.doAction("opendoor")
        else
          @door.doAction("closedoor")
        end
      end
  end
end

# Test case for Doors. Tests basic GameObject interaction,
# ObjectStateChange event handling and physical collision updates.
class DoorTest < TestCase
    def initialize(center)
        super(center)

        # Define a base box to place everything else on.
        height = 0.3
        min_base = [-3.0, 0.05, -3.0]
        max_base = [3.0, height, 3.0]
        base = $AM.createBoxPrimitiveActor("DoorTestBase", min_base, max_base)
        base.placeIntoScene(center);

        # The door with the door knob
        door1 = $GOM.createGameObjectProxy("door").getGameObject();
	door1.setDescription("Eine Holztuer");
	door1.setPosition(rel_pos([1.5, height, 0.0]));
        # Place door and add door actions. Initially the door is not open but can be opened.
        door1.placeIntoScene()

        # The door without a working door knob, to be opened with
        door2 = $GOM.createGameObjectProxy("door").getGameObject();
	door2.setDescription("Eine Holztuer.\nDiese Tuer hat weder Klinke noch Schloesser");
	door2.setPosition(rel_pos([-1.5, height, 0.0]));
        # Place door and add door actions. Initially door is not open and cannot be opened.
        door2.placeIntoScene()

        # The switch to open the second door
        switch = $GOM.createGameObjectProxy("switch3way").getGameObject();
        switch.setPosition(rel_pos([2.25, height, 2.0]));
	switch.placeIntoScene();

        doorprocessor = DoorOpener.new(door2, switch);

        $SCRIPT.log("doortest initialisiert");
    end
end

