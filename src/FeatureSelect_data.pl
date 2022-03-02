#!/usr/bin/perl
use Getopt::Long qw(:config no_ignore_case);
use Cwd 'abs_path';
use File::Basename;
use strict;
use warnings;
my $program = abs_path($0);
my $data;
my $tab;
my $help;
sub Usage(){
	print
"Usage perl $program [option]
option:
	-data	[FILE] all features file
	-tab	[FILE] ibcga results file
	-h, -help	Show the usage
";
}
GetOptions(
 	'data=s'     =>\ $data,
	'tab=s'	=>\ $tab,
 	'help|h'        =>\ $help,
 );
if($help||!$data||!$tab){
	&Usage();
	exit;
}
my $tabname = basename($tab);
my $data_select = $data."_".$tabname;
open OUT,">",$data_select;
my $feature_tag = 0;
my @arrayFeature;
my $NumFeatures = 0;
open TAB,"<",$tab;
while(my $line=<TAB>){
	chomp $line;
	if($line =~ 'Features'){
		my @tmp = split(/ /,$line);
		$NumFeatures = $tmp[1];
		print STDERR "NumFeatures:$NumFeatures\n";
		$feature_tag = 1;
		next;
	}
	if($feature_tag){
		$line =~ s/ //g;
		print STDERR "$line\n";
		my @tmp = split(/,/,$line);
		my $i = 0;
		foreach my $x (@tmp){
			$arrayFeature[$i] = $x;
			$i++;
		}
	}
}
close TAB;

open DATA,"<",$data;
while(my $line=<DATA>){
	chomp $line;
	my @tmp = split(/ /,$line);
	my $no;
	print OUT $tmp[0]." ";	
	for(my $i=0;$i<$NumFeatures;$i++){
		$no = $i+1;
		my $No_feature = $arrayFeature[$i];
		my @value = split(/:/,$tmp[$No_feature]);
		print OUT "$no:$value[1] ";
	}
	print OUT "\n";
}
close DATA;
close OUT;