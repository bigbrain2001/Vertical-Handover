simple FLC
{
	parameters: 
		//xml config= xmldoc("HP_rules.xml");
		xml config;
		int networkSize;
	gates:
		input rxServer[networkSize];
		output txServer;
}

 