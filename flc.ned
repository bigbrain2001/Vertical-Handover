simple FLC
{
	parameters: 
		xml config= xmldoc("HP_rules.xml");
		int networkSize;
	gates:
		input rxServer[networkSize];
		output txServer;
}

 