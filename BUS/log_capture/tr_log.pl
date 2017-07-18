#marko bovellan 2009
#simply split one row log to multiple rows based on pipe mark
#NOTE! use with care, replaces original file
use Getopt::Std;

my $filename;
my $temp_file = "temp.txt";

#**********************************************************************************
# Argument handling
getopts( "f:", \%arguments );

if ( $arguments{f} ) #file obligatory
{
  $filename = $arguments{f};
}
else
{
  print"Usage: perl (-S) tr_log.pl -f <filename>\n";
  print"NOTE, use with care, replaces original file!\n";
  exit;
}

open (READ_FILE, "$filename") or die "\nCan't open input file $filename!\n";
open (WRITE_FILE, ">$temp_file") or die "\nCan't open output file $temp_file!\n";
while(<READ_FILE>)
{ 
  #Nov 11 09:08:32 notice CLA-0 bc2prb: IL:FAMILY ID:1521 PROCESS ID:1 FOCUS ID:3|WRITE TIME:2009-11-11 09:08:32:909|UNIT:OMU-0|NUM:9056|PARAMETERS:T ERROR LAYER TEXT|COUNT:65|USER TEXT:|USER DATA:P07:Not found bothway partner. | termination id:    = 01000000208BE0  | context:           = 0x34002 
  tr/|/\n/;
  print WRITE_FILE $_ . "\n";
}  
close READ_FILE;
close WRITE_FILE;
unlink("$filename");
system ("mv $temp_file $filename");

