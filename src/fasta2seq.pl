#!/usr/bin/perl
use strict;
use warnings;
use File::Basename;
my $fa = $ARGV[0];
my $out_dir = $ARGV[1];
if(@ARGV < 2){
    print STDERR "fastq2seq.pl fasta out_dir\n";die;
}
my $init = 0;
my $amino_acids = "ACDEFGHIKLMNPQRSTVWY";
my @amino = split("",$amino_acids);
my %amino;
foreach my $i (@amino){
    $amino{$i}++;
}
$out_dir =~ s/\/$//;
my $fa_name = basename($fa);
my $out = $out_dir."/".$fa_name.".seq";
open OUT,">",$out;
open FA,"<",$fa;
while(my $line=<FA>){
    chomp $line;
    $line =~ s/\r//g;
    if($line =~ /^>/){
        if(!$init){
             next;
        }else{
            print OUT "\t0\n";
        }
        next;
    }
    my @tmp = split("",$line);
    foreach my $i (@tmp){
        if($amino{$i}){
           print OUT $i; 
        }
    }
    #print OUT $line;
    $init = 1;
    
}
close FA;

print OUT "\t0\n";
close OUT;
