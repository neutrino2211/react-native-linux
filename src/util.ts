// Helper to manage C node pointers
export class CNode {
  ptr: number;

  static fromPtr(ptr: number): CNode {
    const n = new CNode();
    n.ptr = ptr;
    return n
  }
}