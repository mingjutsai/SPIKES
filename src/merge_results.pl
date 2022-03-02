#!/usr/bin/perl
use strict;
use warnings;
my $fa = $ARGV[0];
my $results = $ARGV[1];
my $out_dir = $ARGV[2];
if(@ARGV < 3){
    print STDERR "fastq2seq.pl fasta predict_results out_dir\n";die;
}
my @pred;
my $no = 0;
open RE,"<",$results;
while(my $line=<RE>){
    chomp $line;
    if($line =~ /^labels/){
        next;
    }
    my @ele = split(/ /,$line);
    #print STDERR $no.":".$ele[2]."\n";
    $pred[$no] = $ele[2];
    $no++;
}
close RE;
my $init = 0;

my $n = 0;
$out_dir =~ s/\/$//;
my $out = $out_dir."/SPIKES_prediction_results.csv";
open OUT,">",$out;
print OUT "Header,Sequence,Prediction Score\n";
open FA,"<",$fa;
while(my $line=<FA>){
    chomp $line;
    $line =~ s/\r//g;
    if($line =~ /^>/){
        if(!$init){
             print OUT $line.",";
             next;
        }else{
            #print STDERR $n."\n";
            print OUT ",".$pred[$n]."\n";
            print OUT $line.",";
            $n++;
        }
        next;
    }
    print OUT $line;
    $init = 1;
    
}
close FA;
print OUT ",".$pred[$n]."\n";
close OUT;
