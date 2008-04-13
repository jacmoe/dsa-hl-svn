require 'npc.rb'
require 'torch.rb'

hero = $UI.getActiveCharacter();

itemToAdd = $GOM.createGameObject("men_human_female_leg_pants_01_long");
hero.getInventory().hold(itemToAdd, "Trousers");

itemToAdd = $GOM.createGameObject("men_human_female_feet_boots_01");
hero.getInventory().hold(itemToAdd, "Boots");

itemToAdd = $GOM.createGameObject("torch");
hero.getInventory().hold(itemToAdd, "Right Hand");
