function getNumberOfDays($year:int, $month:int):int
{
	var month:Date = new Date($year, $month + 1, 0);
	return month.date;
}
 
//jan
trace(getNumberOfDays(2010, 0));  // traces 31
 
//feb
trace(getNumberOfDays(2010, 1)); // traces 28
 
//dec
trace(getNumberOfDays(2010, 11)); // traces 31