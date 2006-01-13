load "embed.rb"

require 'player.rb'
require 'torch.rb'
require 'hero.rb'
require 'door.rb'
require 'switch.rb'
require 'truhe.rb'
require 'door_opener.rb'
require 'heiltrank.rb'
require 'crate.rb'

# Physik aktivieren
$PM.setEnabled(true);

$SCRIPT.log("init map 'minidemo'...");
$World = $CORE.getWorld()
$World.setSkyBox(true, "rl/dsa07")
$SCRIPT.log("skybox set");

$SCRIPT.log("Tageslicht erstellen..");
sunlight = $AM.createLightActor("sunlight", LightObject::LT_DIRECTIONAL);
sunlight.getControlledObject().setDirection(-0.8, -2.0, 3.0);
sunlight.getControlledObject().setCastShadows(false);
sunlight.getControlledObject().setDiffuseColour(0.8,0.8,0.7);
$SCRIPT.log("Tageslicht erstellt.");

$SCRIPT.log("Held erstellen");
$hero = Hero.new;
$SCRIPT.log("Held erstellt");
$SCRIPT.log("Held in die Szene einfuegen.");
$hero.getActor().placeIntoScene(0.0, 20.0, 40.0, 1.0, 0.0, 0.0, 0.0);
$SCRIPT.log("Held eingefügt.");
PlayerSettings.preparePlayer($hero);
$SCRIPT.log("Held vorbereitet.");
$UI.setActiveCharacter($hero);
$SCRIPT.log("Held als aktiver Charakter gesetzt.");

$SCRIPT.log("Kiste einsetzen");
crate = Crate.new("Eine Kiste", "Eine Kiste.\nWas mag da drin sein?");
crate.getActor().placeIntoScene(180.0, 80.0, 220.0, 1.0, 0.0, 0.0, 0.0);

$SCRIPT.log("Türen einsetzen")
door1 = Door.new("Eine Tür links", "Eine Holztür", false, true);
door1.getActor().placeIntoScene(1368.0, -75.0, -170.0, 1.0, 0.0, 0.0, 0.0);
door1.getActor().yaw(-90.0)

gtuer = Door.new("Eine Tür rechts", "Eine Holztür.\nDiese Tür hat weder Klinke noch Schlösser", false, false);
gtuer.getActor().placeIntoScene(1368.0, -75.0, 60.0, 1.0, 0.0, 0.0, 0.0)
gtuer.getActor().yaw(-90.0)
$SCRIPT.log("Türen fertig")


$SCRIPT.log("Hebel reinsetzen")
hebel = Switch.new( "Hebel" );
hebel.getActor().placeIntoScene( 1330.0, 25.0, 180.0, 1.0, 0.0, 0.0, 0.0 );
hebel.getActor().pitch(90.0)
hebel.getActor().roll(90.0)
$SCRIPT.log("Hebel fertig");

doorprocessor = SecretDoorOpener.new(gtuer, hebel)

$SCRIPT.log("Truhe reinsetzen")
chest = Chest.new( "Truhe", false );
chest.getActor().placeIntoScene( 1750.0, -350.0, -105.0, 1.0, 0.0, 0.0, 0.0 );
chest.getActor().yaw(-90.0);
$SCRIPT.log("Truhe fertig");

$SCRIPT.log("Traenke reinsetzen")
trank = Heiltrank.new( "Trank", "Eine Flasche mit einer geheimnisvollen Fluessigkeit", "obj_heiltrank01.mesh", "A" );
chest.addItem(trank);
trank = Heiltrank.new( "Trank", "Eine andere Flasche mit einer anderen geheimnisvollen Fluessigkeit", "obj_heiltrank01.mesh", "B" );
chest.addItem(trank);
$SCRIPT.log("Traenke fertig");

#$SCRIPT.log("Sound laden");
#LevelLied = $AM.createSoundSampleActor( "LevelLied", "spannend.ogg" );
#$SCRIPT.log(" Platzieren");
#LevelLied.placeIntoScene( 160.0, 24.0, 160.0, 1.0, 0.0, 0.0, 0.0 );
#$SCRIPT.log(" Loopen");
#LevelLied.getControlledObject().setLooping( true );
#$SCRIPT.log(" Abspielen");
#LevelLied.getControlledObject().play();
#$SCRIPT.log("Sound fertig");

$World.setFog( World::FOG_EXP, [0.8,0.8,1.0,0.5], 0.00009, 0.2, 1.0);

load "kreislauf.rb"
load "timer.rb"
load "area.rb"

$SCRIPT.log("map 'minidemo' initialisiert.");
