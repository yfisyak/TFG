#! /usr/bin/env perl
use File::Basename;
#my $glob =  "/net/l404/data/fisyak/Pico/BES-I/AuAu19_production/2011/???/*";
#my $glob =  "/net/l401/data/scratch2/kehw/reco/2019/???/*";
my $glob =  "/net/l401/data/scratch2/kehw/reco/2019/TFG19d//???/*";
foreach my $file (glob $glob) {
  my $f = File::Basename::basename($file);
  my $pico = $f . ".root";
  if ( -r $pico) {next};
  print "string:$file\n";
#  last;
}
