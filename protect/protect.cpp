
//#include <string>
//
//std::string GetHDid()
//{
//	std::string HDid = "";
//	ManagementClass cimobject1 = new ManagementClass("Win32_DiskDrive");
//	ManagementObjectCollection moc1 = cimobject1.GetInstances();
//	foreach(ManagementObject mo in moc1)
//	{
//		HDid = (string)mo.Properties["Model"].Value;
//	}
//	return HDid.ToString();
//}