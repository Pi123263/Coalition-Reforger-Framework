[ComponentEditorProps(category: "Game Mode Component", description: "")]
class CRF_SupplyExtractionGameModeComponentClass: SCR_BaseGameModeComponentClass
{
	
}

class CRF_SupplyExtractionGameModeComponent: SCR_BaseGameModeComponent
{
	[Attribute("2", "auto", "The amount of supply depots the extracting team has to extract to.")]
	int totalDepots;
	
	[Attribute("2000", "auto", "Total supplies extracted to win.")]
	int winningSupplyCount;
	
	[Attribute("8", "auto", "Manpower needed for retreat.")]
	int manpower;
	
	[Attribute("100", "auto", "Meters from supply depot extracting sides needs to be to extract.")]
	int extractionDistance;
	
	[Attribute("extractionObject", "auto", "Name of object at extraction.")]
	string extractionObject;
	
	[Attribute("USSR", "auto", "Faction key of side extracting supplies.")]
	string factionKey;
	
	[Attribute("true", "auto", "Enables manpower message.")]
	bool enableManpowerMessage;
	
	[Attribute("Soviets forces do not have the manpower to retreat!", "auto", "Message given to players when the extracting force doesn't have the manpower to retreat.")]
	string manpowerMessageString;
	
	[Attribute("true", "auto", "Enables supplies extracted message.")]
	bool enableSuppliesExtracted;
	
	[Attribute("All Soviet Supplies have been evacuated from Tyrone!", "auto", "Message given to players when the extracting force has extracted enough supplies to retreat.")]
	string suppliesExtractedString;
	
	[Attribute("true", "auto", "Enables message given to players when the extracting force has retreated.")]
	bool enableGameMessage1;
	
	[Attribute("Soviets have successfully extracted from the AO!", "auto", "Message given to players when the extracting force has retreated.")]
	string gameMessageString1;
	
	[Attribute("true", "auto", "Enables message given to players when the extracting force has extracted all supplies but got wiped out.")]
	bool enableGameMessage2;
	
	[Attribute("Soviets have extracted all resources but have been decisively defeated!", "auto", "Message given to players when the extracting force has extracted all supplies but got wiped out.")]
	string gameMessageString2;
	
	[Attribute("true", "auto", "Enables message given to players when the extracting force has been wiped out.")]
	bool enableGameMessage3;
	
	[Attribute("Soviets forces have been decisively defeated!", "auto", "Message given to players when the extracting force has been wiped out.")]
	string gameMessageString3;
	
	protected int totalSupply;
	protected ref array<IEntity> m_entities;
	protected string m_tempString;
	protected IEntity extractionObjectGame;
	protected vector extractionLocation;
	protected int extractedPlayers;
	protected bool manpowerMessage;
	protected bool supplyMessage;
	protected bool gameMessage1;
	protected bool gameMessage2;
	protected bool gameMessage3;
	//------------------------------------------------------------------------------------------------
	override protected void OnWorldPostProcess(World world)
	{
		if (!GetGame().InPlayMode()) 
			return;
		FactionManager fm = GetGame().GetFactionManager();
  		Faction faction = fm.GetFactionByKey(factionKey);
		extractionLocation = GetGame().GetWorld().FindEntityByName(extractionObject).GetOrigin();
		GetGame().GetCallqueue().CallLater(GameMessages, 1000, true);
	}
	
	protected bool GetSuppliesinDepot()
	{
		if (!SCR_BaseGameMode.Cast(GetGame().GetGameMode()).IsRunning()) return false;
		for (int i = 1; i <= totalDepots; i++)
		{
			string depot = "depot" + i;
			IEntity depotObj = GetGame().GetWorld().FindEntityByName(depot);
			SCR_ResourceComponent rc = SCR_ResourceComponent.FindResourceComponent(depotObj, false);
			float depotSupply = 0.0;
			SCR_ResourceSystemHelper.GetStoredResources(rc, depotSupply);
			totalSupply = totalSupply + depotSupply;
			Print(totalSupply);
		}
		if (totalSupply >= winningSupplyCount) 
		{
			totalSupply = 0;
			return true;
		}
		totalSupply = 0;
		return false;
	}	
	
	protected bool FilterFactionPlayers(IEntity ent) // Sphere Query Method
	{
		if (!EntityUtils.IsPlayer(ent)) return true;
		SCR_ChimeraCharacter cc = SCR_ChimeraCharacter.Cast(ent);
		if (cc && cc.GetFactionKey() == m_tempString) m_entities.Insert(ent);
		return true;
	}
	
	protected int CountFactionPlayers(vector center, int distance, string faction)
	{
		m_entities = new array<IEntity>;
		m_tempString = faction;
		GetGame().GetWorld().QueryEntitiesBySphere(center, distance, FilterFactionPlayers, null);
		extractedPlayers = m_entities.Count();
		return m_entities.Count();
	}
	
	static int countAlivePlayers(string filter)
	{
		int players = 0;
		array<int> outPlayers = {};
		GetGame().GetPlayerManager().GetPlayers(outPlayers);
		foreach(int playerId : outPlayers)
		{
		 	PlayerController pc = GetGame().GetPlayerManager().GetPlayerController(playerId); if (!pc) continue;
		  	IEntity controlled = pc.GetControlledEntity();
		  	SCR_ChimeraCharacter ch = SCR_ChimeraCharacter.Cast(controlled); if (!ch) continue;
			CharacterControllerComponent ccc = ch.GetCharacterController();
			if (filter == ch.GetFactionKey() && !ccc.IsDead()) ++players;
		}
		return players;
	}
	
	protected void GameMessages()
	{
		if (!SCR_BaseGameMode.Cast(GetGame().GetGameMode()).IsRunning()) return;
		int playerCount = CountFactionPlayers(extractionLocation, extractionDistance, factionKey);
		int overallCount = countAlivePlayers(factionKey);
		if (GetSuppliesinDepot() == true && !supplyMessage && enableSuppliesExtracted)
		{
			SCR_PopUpNotification.GetInstance().PopupMsg(suppliesExtractedString, 10);
			supplyMessage = true;
		}
		if (overallCount < manpower && !manpowerMessage && enableManpowerMessage) 
        {
            SCR_PopUpNotification.GetInstance().PopupMsg(manpowerMessageString, 10);
			manpowerMessage = true;
        }	
		if (GetSuppliesinDepot() && playerCount >= manpower && !gameMessage1 && enableGameMessage1) 
		{
			SCR_PopUpNotification.GetInstance().PopupMsg(gameMessageString1, 10);
			gameMessage1 = true;
		}
		
		if (GetSuppliesinDepot() && overallCount == 0 && !gameMessage2 && enableGameMessage2) 
		{
			SCR_PopUpNotification.GetInstance().PopupMsg(gameMessageString2, 10);
			gameMessage2 = true;
		}
		
		if (GetSuppliesinDepot() == false && overallCount == 0 && !gameMessage3 && enableGameMessage3) 
		{
			SCR_PopUpNotification.GetInstance().PopupMsg(gameMessageString3, 10);
			gameMessage3 = true;
		}
	}
}