import mido
from mido import MidiFile, MidiTrack, Message
import re
from collections import defaultdict

# Textdatei einlesen
with open('toMidi.txt', 'r') as f:
    lines = f.readlines()

# Hilfsfunktion um eine Zeile zu parsen
def parse_line(line):
    matches = re.findall(r'vc(\d+)_c(\d+)_s(\d+)_l(\d+)_n(\d+)_v(\d+)', line)
    if matches:
        v, channel, start, length, note, velocity = map(int, matches[0])
        return {
            'voice': v,
            'channel': channel,
            'start': start,
            'length': length,
            'note': note,
            'velocity': velocity
        }
    return None

# Events pro Channel sammeln
events_per_channel = defaultdict(list)

for line in lines:
    if line.startswith('vc'):
        event = parse_line(line)
        if event:
            events_per_channel[event['channel']].append(event)

# MIDI-Datei erstellen
mid = MidiFile(type=1)  # Format 1 = mehrere parallele Tracks
ppqn = 24
mid.ticks_per_beat = ppqn

# Für jeden Kanal eine eigene Spur erstellen
for channel, events in events_per_channel.items():
    track = MidiTrack()
    mid.tracks.append(track)

    all_events = []

    # Note On und Note Off Events erstellen
    for event in events:
        # Note On
        all_events.append( (event['start'], Message('note_on',
                                                    channel=channel-1,  # ACHTUNG: channel-1, wie im ersten Script
                                                    note=event['note'],
                                                    velocity=event['velocity'])) )

        # Note Off
        all_events.append( (event['start'] + event['length'], Message('note_off',
                                                                      channel=channel-1,
                                                                      note=event['note'],
                                                                      velocity=0)) )

    # Events nach Zeitpunkt sortieren
    all_events.sort(key=lambda e: e[0])

    current_tick = 0

    # Events mit Delta-Zeiten einfügen
    for tick, msg in all_events:
        delta = tick - current_tick
        if delta < 0:
            delta = 0
        msg.time = delta
        track.append(msg)
        current_tick = tick

    # End of Track
    track.append(mido.MetaMessage('end_of_track', time=0))

# Speichern
mid.save('output_fixed_tracks.mid')
print('Fertig! MIDI gespeichert als output_fixed_tracks.mid')
