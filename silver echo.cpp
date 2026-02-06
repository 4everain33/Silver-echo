import jieba
import os
import random
from PIL import Image

# --- CONFIGURATION ---
ASSETS_PATH = 'assets' 
OUTPUT_SIZE = (1080, 1080)
# Input Diary: "The weather is great, I'm going to the garden to admire the flowers."
DIARY_TEXT = "天气很好，我出去花园赏花。" 

# --- PATTERN DATABASE ---
# Maps Pattern IDs to keywords (Environment, Mood, Objects)
PATTERN_DB = {
    "1": ["butterfly", "insect", "garden", "flying", "freedom"],
    "2": ["spiral", "vortex", "cycle", "infinite", "abstract"],
    "3": ["flower", "plant", "bloom", "nature", "garden"],
    "4": ["moon", "crescent", "night", "peace", "sky"],
    "5": ["raindrop", "tear", "water", "purity", "dew"],
    "6": ["sun", "circle", "warmth", "energy", "sunlight"],
    "7": ["cloud", "sky", "weather", "lucky", "floating"],
    "8": ["ornament", "complex", "decoration", "traditional"],
    "9": ["petal", "symmetry", "flower", "calm", "garden"],
    "10": ["turtle", "longevity", "steady", "protection"],
    "11": ["vine", "growth", "climbing", "life", "twisting"],
    "12": ["totem", "center", "order", "ritual", "focus"],
    "13": ["frog", "vitality", "spring", "jump", "nature"],
    "14": ["weave", "structure", "ethnic", "handmade"],
    "15": ["tassel", "pendant", "hanging", "elegant", "ethnic"],
    "16": ["vessel", "container", "artifact", "ancient", "pot"],
    "17": ["ear", "hearing", "link", "abstract", "connection"],
    "18": ["ring", "link", "complete", "space", "circular"],
    "19": ["bat", "mystery", "night", "blessing", "flying"],
    "20": ["sunflower", "optimism", "sun", "garden", "bloom"],
    "21": ["fish", "water", "swimming", "freedom", "wealth"],
}

class PatternGenerator:
    def __init__(self, assets_path, output_size):
        self.assets_path = assets_path
        self.output_size = output_size
        
        if not os.path.exists(self.assets_path):
            print(f"Error: Folder '{self.assets_path}' not found.")

    def analyze_diary(self, text):
        """Extracts keywords and matches them to Pattern IDs."""
        words = jieba.lcut(text)
        matched_ids = set()
        
        for img_id, tags in PATTERN_DB.items():
            for word in words:
                if word in tags:
                    matched_ids.add(img_id)
        
        # Fallback to default patterns if no match is found
        return list(matched_ids) if matched_ids else ["2", "12", "18"]

    def generate_motif(self, text):
        """Generates a new pattern layout based on diary sentiment and content."""
        matched_patterns = self.analyze_diary(text)
        
        # Create a blank white canvas
        canvas = Image.new('RGBA', self.output_size, (255, 255, 255, 255))
        
        print(f"Analyzing: {text}")
        print(f"Matched Patterns: {matched_patterns}")

        # Sentiment density: More words = denser pattern
        density = random.randint(30, 60) 
        
        for _ in range(density):
            p_id = random.choice(matched_patterns)
            img_path = os.path.join(self.assets_path, f"pattern_{p_id}.png")
            
            if not os.path.exists(img_path):
                continue
            
            with Image.open(img_path).convert("RGBA") as p_img:
                # RANDOMIZATION: Rotation, Scale, and Position
                angle = random.randint(0, 360)
                scale = random.uniform(0.3, 0.7)
                new_size = tuple(int(dim * scale) for dim in p_img.size)
                
                # Apply transformation
                p_mod = p_img.resize(new_size, Image.Resampling.LANCZOS).rotate(angle, expand=True)
                
                # Random position (centered scattering)
                x = random.randint(-p_mod.size[0]//2, self.output_size[0] - p_mod.size[0]//2)
                y = random.randint(-p_mod.size[1]//2, self.output_size[1] - p_mod.size[1]//2)
                
                # Composite onto canvas
                canvas.alpha_composite(p_mod, (x, y))
        
        # Save and Show
        canvas.show()
        canvas.save("diary_generated_pattern.png")
        print("Success: Pattern saved as 'diary_generated_pattern.png'")

# --- RUN ---
if __name__ == "__main__":
    gen = PatternGenerator(ASSETS_PATH, OUTPUT_SIZE)
    gen.generate_motif(DIARY_TEXT)