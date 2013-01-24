#!/usr/local/bin/perl

$count = 0;
$cmd = <>;
@mas_cmd = split(" ", $cmd);
$/ = "";

for(0..@mas_cmd-1)
{
	eval{
		open(ID, "<$mas_cmd[$_]");
		$file = <ID>;
		@mas = split(/ /, $file);
		while(@mas)
		{
			$elem = shift(@mas);
			if($elem !~ m/\D/i)
			{
				$buf[$count++] = $elem;
			}
		}
		close(ID);
	};
	print $@;
}
eval{
	@buf = sort{$a <=> $b} @buf;
	open(ID, ">$mas_cmd[$#mas_cmd]");
	print ID join(" ", @buf);
	close(ID);
};
print $@;