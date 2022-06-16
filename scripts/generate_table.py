import os
import struct
from torch import embedding
import tqdm
import argparse 

class TableGenerator:
    def __init__(self,cfg):
        self.emb_dim = cfg.emb_dim
        self.total_entries = cfg.total_entries
        self.save_path = cfg.save_path

    def generate(self):
        embedding_size = self.emb_dim * self.total_entries * 4 / (1024 * 1024 * 1024)
        print("Embedding size: {} GB".format(embedding_size)) 
        print("Save path:",self.save_path)
        with open(self.save_path,'wb') as f:
            for i in tqdm.tqdm(range(self.total_entries)):
                for j in range(self.emb_dim):
                    d = struct.pack('f',float(i))
                    f.write(d)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--emb_dim",
        type=int,
        default=64,
    )

    parser.add_argument(
        "--total_entries",
        type=int,
        default=10000000,
    )

    parser.add_argument(
        "--save_path",
        type=str,
        default="/datasets/tmp/file_cache/values.bin",
    )

    cfg = parser.parse_args()
    generator = TableGenerator(cfg)
    generator.generate()