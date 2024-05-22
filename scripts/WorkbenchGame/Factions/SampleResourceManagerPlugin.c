[WorkbenchPluginAttribute(name: "Sample Resource Manager Plugin",category: "Coalition Reforger Framework", wbModules: { "ResourceManager" })]
class SampleResourceManagerPlugin : WorkbenchPlugin
{
	//settings 
	[Attribute(desc: "Faction Name")]
	protected string m_Name;
	[Attribute(desc: "Side")]
	protected ResourceName m_Side;
	[Attribute(desc: "Default Rifle")]
	protected ResourceName m_defaultRifle;
	[Attribute(desc: "Default Rifle Ammo")]
	protected ResourceName m_defaultRifleAmmo;
	[Attribute(desc: "Default Helmet")]
	protected ResourceName m_defaultHelmet;
	[Attribute(desc: "Default Jacket")]
	protected ResourceName m_defaultJacket;
	[Attribute(desc: "Default Pants")]
	protected ResourceName m_defaultPants;
	[Attribute(desc: "Default Vest")]
	protected ResourceName m_defaultVest;
	[Attribute(desc: "Default MG")]
	protected ResourceName m_defaultMG;
	[Attribute(desc: "Default MG Ammo")]
	protected ResourceName m_defaultMGAmmo;
	[Attribute(desc: "Default LAT")]
	protected ResourceName m_defaultLAT;
	
	
	
	
	
	
	override void Run()
	{
		Print("I'm here!");
		Workbench.ScriptDialog("Plugin script dialog title", "Description of the plugin\nThis description can use multiple lines.", this);
	}
}