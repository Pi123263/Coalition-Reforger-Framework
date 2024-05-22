[WorkbenchPluginAttribute(name: "Sample Resource Manager Plugin", wbModules: { "ResourceManager" })]
class SampleResourceManagerPlugin : WorkbenchPlugin
{
	override void Run()
	{
		Print("I'm here!");
	}
}