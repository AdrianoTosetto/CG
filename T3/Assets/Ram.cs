using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Ram : MonoBehaviour {
	private float cycle;
	private float start;
	public float factor;
	// Use this for initialization
	void Start () {
		start = transform.position.z;
		factor = 500 - (transform.position.x * 10);
	}

	// Update is called once per frame
	void Update () {
		if (factor >= 0) factor = (factor - 0.1f);
		if (factor <= 0) {
			cycle = (cycle + 0.01f) % (2 * Mathf.PI);
			transform.position = new Vector3 (transform.position.x, transform.position.y, start + (15 * Mathf.Sin (cycle)));	
		}
	}
}
