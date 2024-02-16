import csv
import re

def counter(infile, outfile):
    with open(infile) as f:
        reader = csv.reader(f)
        counts = {
            'NOP': {}, 
            'LW': {},  # Unifica LWD e LWI in LW
            'SW': {},  # Unifica SWD e SWI in SW
            'ALU': {}  # Aggiunta della categoria ALU
        }
        current_timestamp = None

        for row in reader:
            match = re.match(r"(\d+)", row[0])
            if match:
                current_timestamp = match.group(1)
                if current_timestamp not in counts['NOP']:
                    for key in counts:
                        counts[key][current_timestamp] = 0  # Inizializzazione per tutte le categorie
            else:
                counts['NOP'][current_timestamp] += row.count("NOP")
                for instr in row:
                    if instr == "NOP":
                        continue  # Escludi NOP dal conteggio ALU
                    elif re.match(r"LW[D|I]\s+\w+", instr) or re.match(r"LW[D|I]\s+\w+,\s*\w+", instr):
                        counts['LW'][current_timestamp] += 1
                    elif re.match(r"SW[D|I]\s+\w+", instr) or re.match(r"SW[D|I]\s+\w+,\s*\w+", instr):
                        counts['SW'][current_timestamp] += 1
                    else:
                        counts['ALU'][current_timestamp] += 1  # Aggiornamento del conteggio ALU

    # Formattazione dell'output
    out_string = "T\t" + "\t".join(counts['NOP'].keys()) + "\n"
    for key in counts:
        out_string += key + "\t" + "\t".join(str(counts[key][timestamp]) for timestamp in counts[key]) + "\n"

    with open(outfile, "w") as f:
        f.write(out_string)
