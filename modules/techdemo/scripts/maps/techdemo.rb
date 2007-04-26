load "embed.rb"

# Physik aktivieren
$PM.setEnabled(true);

$SCRIPT.log("init map 'techdemo'...");
$World = $CORE.getWorld()

$SCRIPT.log("Welten-Thema laden");
techdemoWelt = $AM.createSoundSampleActor( "techdemoWelt", "techdemo001.ogg" ); 
techdemoWelt.getControlledObject().set3d(false);
$SCRIPT.log(" Loopen");
techdemoWelt.getControlledObject().setLooping( true );
$SCRIPT.log(" Abspielen");
techdemoWelt.getControlledObject().play();
$SCRIPT.log("Welten-Thema fertig");

$SCRIPT.log("Get hero")
$hero = $GOM.createGameObjectProxy("hero", 1).getGameObject();
$hero.setPosition([0, 0.7, 0]);
$hero.placeIntoScene();

$SCRIPT.log("Prepare hero for being the active character")
PlayerSettings.preparePlayer($hero)

$SCRIPT.log("Set hero as active character")
$UI.setActiveCharacter($hero)

$World.setFog( World::FOG_EXP, [0.8,0.8,1.0,0.5], 0.00003, 0.4, 1.0);

p "This should not be nil since this GO is placed into scene: " + $GOM.getGameObjectProxy(7).getGameObject().getActor()

tueroeffner = CaveDoorOpener.new(
	$GOM.getGameObjectProxy(7).getGameObject(), 
	$GOM.getGameObjectProxy(8).getGameObject())

truhe = $GOM.getGameObjectProxy(9).getGameObject();
truhe.addItem($GOM.getGameObjectProxy(10).getGameObject())


luftfels1 = $GOM.getGameObjectProxy(11).getGameObject()
luftfels2 = $GOM.getGameObjectProxy(12).getGameObject()
luftfels3 = $GOM.getGameObjectProxy(13).getGameObject()

$SCRIPT.log("Steinschlag: SteinschlagzoneListener erstellen");
areaListener = SteinschlagzoneListener.new(luftfels1, luftfels2, luftfels3 );

felshaufen = $GOM.getGameObjectProxy(14).getGameObject()
p felshaufen
p felshaufen.getActor()
p felshaufen.getActor().getPhysicalThing()
felshaufen.getActor().getPhysicalThing().setContactListener( 
	RockpileContactListener.new($felshaufen, luftfels1, luftfels2, luftfels3) );


$SCRIPT.log("Steinschlag: Kugel-Zentrum Actor erstellen");
kugelDings = $AM.createEmptyActor( "Kugel-Zentrum" );
$SCRIPT.log("Steinschlag: Kugel-Zentrum Actor in die Szene einfügen");
kugelDings.placeIntoScene( [78.1769, 10.1317, 50.9391] );



$SCRIPT.log("Steinschlag: SteinschlagzoneListener hinzufügen");
$GameEveMgr.addSphereAreaListener( kugelDings, 4.000, areaListener, RlScript::QUERYFLAG_PLAYER );

$SCRIPT.log("map 'techdemo' initialisiert.");
$WF.toggleCharacterStateWindow()
