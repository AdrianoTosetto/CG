using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Platform : MonoBehaviour {
	private float cycle;
	private float start;
	// Use this for initialization
	void Start () {
		start = transform.position.x;
	}
	
	// Update is called once per frame
	void Update () {
		cycle = (cycle + 0.01f) % (2 * Mathf.PI);
		transform.position = new Vector3 (start + (7 * Mathf.Sin (cycle)), transform.position.y, transform.position.z);
	}
}
